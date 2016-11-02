#include "AudioFileInfo.h"

namespace TagLibUWP
{
	AudioFileInfo::AudioFileInfo(IStorageFile ^ file, TagLibUWP::Tag ^ tag, AudioProperties ^ properties) :
		file(file), properties(properties)
	{
		Tag = tag;
	}
}