#include "FileStreamRT.h"

#include <collection.h>
#include <ppltasks.h>

using namespace Concurrency;
using namespace Platform;

void readFromFile(IRandomAccessStream^ stream, DataReader^ reader, TagLib::ByteVector& buffer)
{
	auto size = stream->Size;
	auto position = stream->Position;
	auto remaining = size - position;
	if (remaining < buffer.size())
	{
		buffer.resize(remaining);
		if (remaining == 0)
		{
			return;
		}
	}

	create_task(reader->LoadAsync(buffer.size())).wait();
	reader->ReadBytes(ArrayReference<unsigned char>(reinterpret_cast<unsigned char*>(buffer.data()), buffer.size()));
}

void writeToFile(IRandomAccessStream^ stream, DataWriter^ writer, const TagLib::ByteVector& buffer)
{
	if (buffer.size() == 0)
	{
		return;
	}

	auto pData = reinterpret_cast<unsigned char*>(const_cast<char*>(buffer.data()));
	writer->WriteBytes(ArrayReference<unsigned char>(pData, buffer.size()));
	create_task(writer->StoreAsync()).wait();
	create_task(writer->FlushAsync()).wait();
}

namespace TagLib
{
	FileStreamRT::FileStreamRT(IStorageFile^ file, FileAccessMode access) : FileStreamRT(create_task(file->OpenAsync(access)).get(), file->Name)
	{
	}

	FileStreamRT::FileStreamRT(IRandomAccessStream^ stream, Platform::String^ name) : Stream(stream), Name(name)
	{
	}

	FileStreamRT::~FileStreamRT()
	{
		delete Stream;
	}

	FileName FileStreamRT::name() const
	{
		return FileName(Name->Data());
	}

	ByteVector FileStreamRT::readBlock(size_t length)
	{
		if (!this->isOpen())
			return ByteVector();

		ByteVector buffer(length);
		auto reader = ref new DataReader(Stream);
		readFromFile(Stream, reader, buffer);
		reader->DetachStream();
		return buffer;
	}

	void FileStreamRT::writeBlock(const ByteVector & data)
	{
		if (!this->isOpen() || this->readOnly())
			return;

		auto writer = ref new DataWriter(Stream);
		writeToFile(Stream, writer, data);
		writer->DetachStream();
	}

	void FileStreamRT::insert(const ByteVector & data, long long start, size_t replace)
	{
		if (!this->isOpen() || this->readOnly())
			return;

		if (data.size() == replace) {
			seek(start);
			writeBlock(data);
			return;
		}
		else if (data.size() < replace) {
			seek(start);
			writeBlock(data);
			removeBlock(start + data.size(), replace - data.size());
			return;
		}

		// Woohoo!  Faster (about 20%) than id3lib at last.  I had to get hardcore
		// and avoid TagLib's high level API for rendering just copying parts of
		// the file that don't contain tag data.
		//
		// Now I'll explain the steps in this ugliness:

		// First, make sure that we're working with a buffer that is longer than
		// the *differnce* in the tag sizes.  We want to avoid overwriting parts
		// that aren't yet in memory, so this is necessary.

		auto bufferLength = DefaultBufferSize;
		while (bufferLength < data.size() - replace)
		{
			bufferLength += DefaultBufferSize;
		}

		// Set where to start the reading and writing.

		auto readPosition = start + replace;
		auto writePosition = start;

		auto reader = ref new DataReader(Stream);
		auto writer = ref new DataWriter(Stream);

		ByteVector buffer = data;
		ByteVector aboutToOverwrite(bufferLength);

		auto initialSize = length();
		do
		{
			// Seek to the current read position and read the data that we're about
			// to overwrite.  Appropriately increment the readPosition.
			seek(readPosition);
			readFromFile(Stream, reader, aboutToOverwrite);
			readPosition += aboutToOverwrite.size();

			// Seek to the write position and write our buffer. Increment the
			// writePosition.
			seek(writePosition);
			writeToFile(Stream, writer, buffer);
			writePosition += buffer.size();

			// Make the current buffer the data that we read in the beginning.
			buffer = aboutToOverwrite;
		} while (readPosition < initialSize);

		seek(writePosition);
		writeToFile(Stream, writer, buffer);

		reader->DetachStream();
		writer->DetachStream();
	}

	void FileStreamRT::removeBlock(long long start, size_t length)
	{
		if (!this->isOpen() || this->readOnly())
			return;

		ByteVector buffer(DefaultBufferSize);

		auto readPosition = start + length;
		auto writePosition = start;

		auto reader = ref new DataReader(Stream);
		auto writer = ref new DataWriter(Stream);

		while(buffer.size() > 0)
		{
			seek(readPosition);
			readFromFile(Stream, reader, buffer);
			readPosition += buffer.size();

			seek(writePosition);
			writeToFile(Stream, writer, buffer);
			writePosition += buffer.size();
		}

		reader->DetachStream();
		writer->DetachStream();

		truncate(writePosition);
	}

	bool FileStreamRT::readOnly() const
	{
		return !Stream->CanWrite;
	}

	bool FileStreamRT::isOpen() const
	{
		return Stream != nullptr;
	}

	void FileStreamRT::seek(long long offset, Position p)
	{
		if (p == Position::Current)
			offset += tell();
		else if (p == Position::End)
			offset = length() + offset;

		Stream->Seek(offset);
	}

	void FileStreamRT::clear()
	{
		//Meaningless in Windows
	}

	long long FileStreamRT::tell() const
	{
		return Stream->Position;
	}

	long long FileStreamRT::length()
	{
		return Stream->Size;
	}

	void FileStreamRT::truncate(long long length)
	{
		length = min(length, this->length());
		Stream->Size = length;
	}
}
