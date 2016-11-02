#pragma once

#include "AudioPropertiesRT.h"
#include "TagRT.h"

using namespace Windows::Storage;

namespace TagLibUWP
{
	public ref class AudioFileInfo sealed
	{
	public:
		property IStorageFile^ File
		{
			IStorageFile^ get() { return file; }
		}

		property Tag^ Tag;
		property AudioProperties^ Properties
		{
			AudioProperties^ get() { return properties; }
		}

		AudioFileInfo(IStorageFile^ file, TagLibUWP::Tag^ tag, AudioProperties^ properties);

	private:
		IStorageFile^ const file;
		AudioProperties^ const properties;
	};
}
