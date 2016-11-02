#include "TagRT.h"

namespace TagLibUWP
{
	Tag::Tag()
	{
	}

	Tag::Tag(const TagLib::Tag& tag)
	{
		InitializeFromTag(tag);
	}

	void Tag::InitializeFromTag(const TagLib::Tag& tag)
	{
		Album = ref new Platform::String(tag.album().toCWString());
		Artist = ref new Platform::String(tag.artist().toCWString());
		Comment = ref new Platform::String(tag.comment().toCWString());
		Genre = ref new Platform::String(tag.genre().toCWString());
		Title = ref new Platform::String(tag.title().toCWString());
		Track = tag.track();
		Year = tag.year();
	}

	void Tag::UpdateTag(TagLib::Tag& tag)
	{
		tag.setAlbum(TagLib::String(Album->Data()));
		tag.setArtist(TagLib::String(Artist->Data()));
		tag.setComment(TagLib::String(Comment->Data()));
		tag.setGenre(TagLib::String(Genre->Data()));
		tag.setTitle(TagLib::String(Title->Data()));
		tag.setTrack(Track);
		tag.setYear(Year);
	}
}