#pragma once

#include "taglib_export.h"
#include "tiostream.h"

using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

namespace TagLib
{
	class TAGLIB_EXPORT FileStreamRT : public IOStream
	{
	public:
		FileStreamRT(IStorageFile^ file, FileAccessMode access);

		FileStreamRT(IRandomAccessStream^ stream, Platform::String^ name);

		/*!
		* Destroys this FileStreamRT instance.
		*/
		virtual ~FileStreamRT();

		/*!
		* Returns the stream name in the local file system encoding.
		*/
		virtual FileName name() const;

		/*!
		* Reads a block of size \a length at the current get pointer.
		*/
		virtual ByteVector readBlock(size_t length);

		/*!
		* Attempts to write the block \a data at the current get pointer.  If the
		* file is currently only opened read only -- i.e. readOnly() returns true --
		* this attempts to reopen the file in read/write mode.
		*
		* \note This should be used instead of using the streaming output operator
		* for a ByteVector.  And even this function is significantly slower than
		* doing output with a char[].
		*/
		virtual void writeBlock(const ByteVector &data);

		/*!
		* Insert \a data at position \a start in the file overwriting \a replace
		* bytes of the original content.
		*
		* \note This method is slow since it requires rewriting all of the file
		* after the insertion point.
		*/
		virtual void insert(const ByteVector &data, long long start = 0, size_t replace = 0);

		/*!
		* Removes a block of the file starting a \a start and continuing for
		* \a length bytes.
		*
		* \note This method is slow since it involves rewriting all of the file
		* after the removed portion.
		*/
		virtual void removeBlock(long long start = 0, size_t length = 0);

		/*!
		* Returns true if the file is read only (or if the file can not be opened).
		*/
		virtual bool readOnly() const;

		/*!
		* Since the file can currently only be opened as an argument to the
		* constructor (sort-of by design), this returns if that open succeeded.
		*/
		virtual bool isOpen() const;

		/*!
		* Move the I/O pointer to \a offset in the stream from position \a p.  This
		* defaults to seeking from the beginning of the stream.
		*
		* \see Position
		*/
		virtual void seek(long long offset, Position p = Beginning);

		/*!
		* Reset the end-of-stream and error flags on the stream.
		*/
		virtual void clear();

		/*!
		* Returns the current offset within the stream.
		*/
		virtual long long tell() const;

		/*!
		* Returns the length of the stream.
		*/
		virtual long long length();

		/*!
		* Truncates the stream to a \a length.
		*/
		virtual void truncate(long long length);

	private:
		static const size_t DefaultBufferSize = 16 * 1024;

		Platform::String^ const Name;
		IRandomAccessStream^ const Stream;
	};
}
