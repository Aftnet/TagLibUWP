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
	Platform::String^ Tag::trackKey(L"TRACKNUMBER");
	Platform::String^ Tag::yearKey(L"DATE");

	Tag::Tag()
	{
	}

	Tag::Tag(const TagLib::Tag& tag)
	{
		properties = TagLibToPlatformMap(tag.properties());
		Image = Picture::FromPictureMape(tag.pictures());
	}

	void Tag::UpdateTag(TagLib::Tag& tag)
	{
		tag.setProperties(PlatformToTagLibMap(properties));
		tag.setPictures(PictureToPictureMap(Image));
	}

	TagLib::String Tag::PlatformToTagLibString(Platform::String^ value)
	{
		if (value == nullptr || value->IsEmpty())
		{
			return TagLib::String();
		}

		return TagLib::String(value->Data());
	}

	Platform::String^ Tag::TagLibToPlatformString(const TagLib::String& value)
	{
		auto output = ref new Platform::String(value.toCWString());
		return output;
	}

	TagLib::PictureMap Tag::PictureToPictureMap(Picture^ value)
	{
		if (value == nullptr || !value->Valid)
		{
			return TagLib::PictureMap();
		}

		return value->ToPictureMap();
	}

	TagLib::SimplePropertyMap Tag::PlatformToTagLibMap(PlatformPropertyMap^ value)
	{
		TagLib::SimplePropertyMap convertedMap;
		for each (auto item in value)
		{
			auto key = PlatformToTagLibString(item->Key);
			auto value = PlatformToTagLibString(item->Value);
			convertedMap.insert(key, TagLib::StringList(value));
		}

		return convertedMap;
	}

	PlatformPropertyMap^ Tag::TagLibToPlatformMap(const TagLib::SimplePropertyMap& value)
	{
		auto convertedMap = ref new Platform::Collections::Map<Platform::String^, Platform::String^>;
		for (auto pair : value)
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

	Platform::String^ Tag::UIntToPlatformString(unsigned int value)
	{
		auto output = ref new Platform::String(std::to_wstring(value).c_str());
		return output;
	}

	unsigned int Tag::PlatformStringToUInt(Platform::String^ value)
	{
		unsigned int output = 0;
		try
		{
			output = std::stoi(value->Data());
		}
		catch(std::invalid_argument)
		{

		}
		catch (std::out_of_range)
		{

		}

		return output;
	}
}