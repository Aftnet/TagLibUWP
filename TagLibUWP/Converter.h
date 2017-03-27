#pragma once

#include "Picture.h"
#include "tag.h"
#include "toolkit\tpropertymap.h"

#include <collection.h>

namespace TagLibUWP
{
	typedef Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^> IPlatformPropertyMap;
	typedef Platform::Collections::Map<Platform::String^, Platform::String^> PlatformPropertyMap;

	static class Converter
	{
	public:
		static const unsigned int DefaultIntValue = 0;

		Converter();
		~Converter();

		static TagLib::String PlatformToTagLibString(Platform::String^ value);
		static Platform::String^ TagLibToPlatformString(const TagLib::String& value);

		static TagLib::PictureMap PictureToPictureMap(Picture^ value);

		static TagLib::PropertyMap PlatformToTagLibMap(PlatformPropertyMap^ value);
		static PlatformPropertyMap^ TagLibToPlatformMap(const TagLib::SimplePropertyMap& value);

		static Platform::String^ UIntToPlatformString(unsigned int value);
		static unsigned int PlatformStringToUInt(Platform::String^ value);
	};
}

