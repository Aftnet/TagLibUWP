#include "TagRT.h"
#include "toolkit\tpropertymap.h"

namespace TagLibUWP
{
	Tag::Tag()
	{
	}

	Tag::Tag(const TagLib::Tag& tag)
	{
		Album = ref new Platform::String(tag.album().toCWString());
		Artist = ref new Platform::String(tag.artist().toCWString());
		Comment = ref new Platform::String(tag.comment().toCWString());
		Genre = ref new Platform::String(tag.genre().toCWString());
		Title = ref new Platform::String(tag.title().toCWString());
		Track = tag.track();
		Year = tag.year();

		Image = Picture::FromPictureMape(tag.pictures());
	}

	void Tag::UpdateTag(TagLib::Tag& tag)
	{
		tag.properties().clear();

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

	TagLib::PictureMap Tag::PictureToPictureMap(Picture^ input)
	{
		if (input == nullptr || !input->Valid)
		{
			return TagLib::PictureMap();
		}

		return input->ToPictureMap();
	}
}