#include "AudioPropertiesRT.h"

namespace TagLibUWP
{
	AudioProperties::AudioProperties(const TagLib::AudioProperties& source) :
		length(source.length()), lengthInSeconds(source.lengthInSeconds()), lengthInMilliseconds(source.lengthInMilliseconds()),
		bitRate(source.bitrate()), sampleRate(source.sampleRate()), channels(source.channels())
	{
	}
}