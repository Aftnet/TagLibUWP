#include "Converter.h"


namespace TagLibUWP
{
	Converter::Converter()
	{
	}

	Converter::~Converter()
	{
	}

	TagLib::String Converter::PlatformToTagLibString(Platform::String^ value)
	{
		if (value == nullptr || value->IsEmpty())
		{
			return TagLib::String();
		}

		return TagLib::String(value->Data());
	}

	Platform::String^ Converter::TagLibToPlatformString(const TagLib::String& value)
	{
		auto output = ref new Platform::String(value.toCWString());
		return output;
	}

	TagLib::PictureMap Converter::PictureToPictureMap(Picture^ value)
	{
		if (value == nullptr || !value->Valid)
		{
			return TagLib::PictureMap();
		}

		return value->ToPictureMap();
	}

	TagLib::PropertyMap Converter::PlatformToTagLibMap(PlatformPropertyMap^ value)
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

	PlatformPropertyMap^ Converter::TagLibToPlatformMap(const TagLib::SimplePropertyMap& value)
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

	Platform::String^ Converter::UIntToPlatformString(unsigned int value)
	{
		auto output = ref new Platform::String(std::to_wstring(value).c_str());
		return output;
	}

	unsigned int Converter::PlatformStringToUInt(Platform::String^ value)
	{
		unsigned int output = defaultIntValue;
		try
		{
			output = std::stoi(value->Data());
		}
		catch (std::invalid_argument)
		{

		}
		catch (std::out_of_range)
		{

		}

		return output;
	}
}
