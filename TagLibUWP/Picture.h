#pragma once

#include "tpicturemap.h"

namespace TagLibUWP
{
	public ref class Picture sealed
	{
	public:
		property Platform::Array<uint8>^ Bytes;
		property Platform::String^ MIMEType;
		property bool Valid
		{
			bool get() { return !(Bytes == nullptr || Bytes->Length < 1 || MIMEType == nullptr || MIMEType->IsEmpty()); }
		};

		Picture();

	internal:
		Picture(const TagLib::PictureMap& pictureMap);
		TagLib::PictureMap ToPictureMap();

	private:
		const TagLib::Picture::Type DefaultPictureType = TagLib::Picture::Type::FrontCover;

		static const TagLib::Picture* GetPictureFromMapIfPresent(const TagLib::PictureMap& pictureMap, TagLib::Picture::Type pictureType);
	};
}
