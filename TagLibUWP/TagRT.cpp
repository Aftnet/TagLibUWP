#include "TagRT.h"

namespace TagLibUWP
{
	Tag::Tag()
	{
	}

	Tag::Tag(const TagLib::Tag& tag)
	{
		Properties = TagLibToPlatformMap(tag.properties());

		Album = TagLibToPlatformString(tag.album());
		Artist = TagLibToPlatformString(tag.artist());
		Comment = TagLibToPlatformString(tag.comment());
		Genre = TagLibToPlatformString(tag.genre());
		Title = TagLibToPlatformString(tag.title());
		Track = tag.track();
		Year = tag.year();

		Image = Picture::FromPictureMape(tag.pictures());
	}

	void Tag::UpdateTag(TagLib::Tag& tag)
	{
		tag.setProperties(PlatformToTagLibMap(Properties));

		tag.setAlbum(PlatformToTagLibString(Album));
		tag.setArtist(PlatformToTagLibString(Artist));
		tag.setComment(PlatformToTagLibString(Comment));
		tag.setGenre(PlatformToTagLibString(Genre));
		tag.setTitle(PlatformToTagLibString(Title));
		tag.setTrack(Track);
		tag.setYear(Year);

		tag.setPictures(PictureToPictureMap(Image));
	}

	TagLib::String Tag::PlatformToTagLibString(Platform::String^ input)
	{
		if (input == nullptr || input->IsEmpty())
		{
			return TagLib::String();
		}

		return TagLib::String(input->Data());
	}

	Platform::String^ Tag::TagLibToPlatformString(const TagLib::String& input)
	{
		auto output = ref new Platform::String(input.toCWString());
		return output;
	}

	TagLib::PictureMap Tag::PictureToPictureMap(Picture^ input)
	{
		if (input == nullptr || !input->Valid)
		{
			return TagLib::PictureMap();
		}

		return input->ToPictureMap();
	}

	TagLib::SimplePropertyMap Tag::PlatformToTagLibMap(Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ map)
	{
		TagLib::SimplePropertyMap convertedMap;
		for each (auto item in map)
		{
			auto key = PlatformToTagLibString(item->Key);
			auto value = PlatformToTagLibString(item->Value);
			convertedMap.insert(key, TagLib::StringList(value));
		}

		return convertedMap;
	}

	Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>^ Tag::TagLibToPlatformMap(const TagLib::SimplePropertyMap& map)
	{
		auto convertedMap = ref new Platform::Collections::Map<Platform::String^, Platform::String^>;
		for (auto pair : map)
		{
			auto key = TagLibToPlatformString(pair.first);
			auto valList = pair.second;
			if (!valList.isEmpty())
			{
				auto value = TagLibToPlatformString(pair.second.front());
				convertedMap->Insert(key, value);
			}
		}

		return convertedMap;
	}
}