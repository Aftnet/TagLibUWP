#pragma once

#include "tpicturemap.h"

namespace TagLibUWP
{
	public ref class Picture sealed
	{
	public:
		property Platform::Array<uint8>^ Data;
		property Platform::String^ MIMEType;
		property bool Valid
		{
			bool get() { return !(Data == nullptr || Data->Length < 1 || MIMEType == nullptr || MIMEType->IsEmpty()); }
		};

		Picture();

	internal:
		static Picture^ FromPictureMape(const TagLib::PictureMap& pictureMap);
		TagLib::PictureMap ToPictureMap();

	private:
		static const TagLib::Picture::Type DefaultPictureType = TagLib::Picture::Type::FrontCover;

		static const TagLib::Picture* GetPictureFromMapIfPresent(const TagLib::PictureMap& pictureMap, TagLib::Picture::Type pictureType);
	};
}
