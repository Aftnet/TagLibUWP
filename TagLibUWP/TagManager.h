#pragma once

#include "fileref.h"
#include "AudioFileInfo.h"

using namespace Platform;
using namespace Windows::Storage;

namespace TagLibUWP
{
	public ref class TagManager sealed
	{
	public:
		property static Array<String^>^ SupportedExtensions {Array<String^>^ get() { return supportedExtensions; }}
		static AudioFileInfo^ ReadFile(IStorageFile^ file);
		static void WriteFile(AudioFileInfo^ fileinfo);
		static void WriteFile(IStorageFile^ file, Tag^ tag);

	private:
		property static String^ UnsupportedFileFormatErrorMsg {String^ get() { return "Unsupported file format"; }}
		property static String^ InvalidFileErrorMsg {String^ get() { return "Invalid or corrupted file"; }}

		static Array<String^>^ const supportedExtensions;
		static Array<String^>^ GetSupportedExtensions();

		static void ValidateFile(const TagLib::FileRef& file);
	};
}
