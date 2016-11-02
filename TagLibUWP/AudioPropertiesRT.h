#pragma once

#include "audioproperties.h"

namespace TagLibUWP
{
	public ref class AudioProperties sealed
	{
	public:
		property int Length
		{
			int get() { return length; }
		}

		property int LengthInSeconds
		{
			int get() { return lengthInSeconds; }
		}

		property int LengthInMilliseconds
		{
			int get() { return lengthInMilliseconds; }
		}

		property int BitRate
		{
			int get() { return bitRate; }
		}

		property int SampleRate
		{
			int get() { return sampleRate; }
		}

		property int Channels
		{
			int get() { return channels; }
		}

	internal:
		AudioProperties(const TagLib::AudioProperties& source);

	private:
		int length;
		int lengthInSeconds;
		int lengthInMilliseconds;
		int bitRate;
		int sampleRate;
		int channels;
	};
}
