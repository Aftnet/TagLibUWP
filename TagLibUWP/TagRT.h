#pragma once

#include "Picture.h"

#include "tag.h"

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

		property Picture^ Image { Picture^ get() { return image; } }

		Tag();

	internal:
		Tag(const TagLib::Tag& tag);
		void UpdateTag(TagLib::Tag& tag);

	private:
		Picture^ image;
	};
}