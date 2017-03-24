#include "TagRT.h"

namespace TagLibUWP
{
	Platform::String^ Tag::albumKey(L"ALBUM");
	Platform::String^ Tag::albumArtistKey(L"ALBUMARTIST");
	Platform::String^ Tag::artistKey(L"ARTIST");
	Platform::String^ Tag::commentKey(L"COMMENT");
	Platform::String^ Tag::composerKey(L"COMPOSER");
	Platform::String^ Tag::copyrightKey(L"COPYRIGHT");
	Platform::String^ Tag::discNumberKey(L"DISCNUMBER");
	Platform::String^ Tag::genreKey(L"GENRE");
	Platform::String^ Tag::titleKey(L"TITLE");
	Platform::String^ Tag::trackNumberKey(L"TRACKNUMBER");
	Platform::String^ Tag::yearKey(L"DATE");

	Tag::Tag()
	{
	}

	Tag::Tag(const TagLib::Tag& tag)
	{
		properties = Converter::TagLibToPlatformMap(tag.properties());
		Image = Picture::FromPictureMape(tag.pictures());
	}

	void Tag::UpdateTag(TagLib::Tag& tag)
	{
		tag.setProperties(Converter::PlatformToTagLibMap(properties));
		tag.setPictures(Converter::PictureToPictureMap(Image));
	}

	Platform::String^ Tag::GetPropertyValue(Platform::String^ key)
	{
		auto fallbackOutput = ref new Platform::String();
		if (properties->HasKey(key))
		{
			auto output = properties->Lookup(key);
			return output == nullptr ? fallbackOutput : output;
		}

		return fallbackOutput;
	}
}