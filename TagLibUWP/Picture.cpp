#include "Picture.h"

#include <memory>

using namespace std;

namespace TagLibUWP
{
	Picture::Picture()
	{
	}

	Picture::Picture(const TagLib::PictureMap& pictureMap)
	{
		auto picture = GetPictureFromMapIfPresent(pictureMap, DefaultPictureType);
		if (picture == nullptr)
		{
			picture = GetPictureFromMapIfPresent(pictureMap, TagLib::Picture::Type::BackCover);
		}

		if (picture == nullptr)
			return;

		auto pictureData = picture->data();
		auto dataPtr = reinterpret_cast<uint8*>(pictureData.data());
		Bytes = ref new Platform::Array<uint8>(dataPtr, pictureData.size());
		MIMEType = ref new Platform::String(picture->mime().toCWString());
	}

	TagLib::PictureMap Picture::ToPictureMap()
	{
		auto dataPtr = reinterpret_cast<char*>(Bytes->Data);
		TagLib::Picture picture(TagLib::ByteVector(dataPtr, Bytes->Length), DefaultPictureType, TagLib::String(MIMEType->Data()));

		TagLib::PictureMap output;
		output.insert(picture);
		return output;
	}

	const TagLib::Picture* Picture::GetPictureFromMapIfPresent(const TagLib::PictureMap& pictureMap, TagLib::Picture::Type pictureType)
	{
		if (pictureMap.contains(pictureType))
		{
			return &pictureMap[pictureType].front();
		}

		return nullptr;
	}

}

