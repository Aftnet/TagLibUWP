#pragma once

#include "Picture.h"

#include "tag.h"
#include "toolkit\tpropertymap.h"

#include <collection.h>

namespace TagLibUWP
{
	public ref class Tag sealed
	{
	public:
		property Platform::String^ Album;
		property Platform::String^ Artist;
		property Platform::String^ Comment;
		property Platform::String^ Genre;
		property Platform::String^ Title;
		property unsigned int Track;
		property unsigned int Year;

		property Picture^ Image;

		property Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ Properties;

		Tag();

	internal:
		Tag(const TagLib::Tag& tag);
		void UpdateTag(TagLib::Tag& tag);

	private:
		TagLib::String PlatformToTagLibString(Platform::String^ input);
		Platform::String^ TagLibToPlatformString(const TagLib::String& input);

		TagLib::PictureMap PictureToPictureMap(Picture^ input);

		TagLib::SimplePropertyMap PlatformToTagLibMap(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ map);
		Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ TagLibToPlatformMap(const TagLib::SimplePropertyMap& map);
	};
}