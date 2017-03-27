#pragma once

#include "Converter.h"

#include "Picture.h"
#include "tag.h"
#include "toolkit\tpropertymap.h"

#include <collection.h>

namespace TagLibUWP
{
	public ref class Tag sealed
	{
	public:
		static property Platform::String^ AlbumKey { Platform::String^ get() { return albumKey; }}
		property Platform::String^ Album
		{
			Platform::String^ get() { return GetPropertyValue(albumKey); }
			void set(Platform::String^ value) { SetPropertyValue(albumKey, value); }
		}

		static property Platform::String^ AlbumArtistKey { Platform::String^ get() { return albumArtistKey; }}
		property Platform::String^ AlbumArtist
		{
			Platform::String^ get() { return GetPropertyValue(albumArtistKey); }
			void set(Platform::String^ value) { SetPropertyValue(albumArtistKey, value); }
		}

		static property Platform::String^ ArtistKey { Platform::String^ get() { return artistKey; }}
		property Platform::String^ Artist
		{
			Platform::String^ get() { return GetPropertyValue(artistKey); }
			void set(Platform::String^ value) { SetPropertyValue(artistKey, value); }
		}

		static property Platform::String^ CommentKey { Platform::String^ get() { return commentKey; }}
		property Platform::String^ Comment
		{
			Platform::String^ get() { return GetPropertyValue(commentKey); }
			void set(Platform::String^ value) { SetPropertyValue(commentKey, value); }
		}

		static property Platform::String^ ComposerKey { Platform::String^ get() { return composerKey; }}
		property Platform::String^ Composer
		{
			Platform::String^ get() { return GetPropertyValue(composerKey); }
			void set(Platform::String^ value) { SetPropertyValue(composerKey, value); }
		}

		static property Platform::String^ CopyrightKey { Platform::String^ get() { return copyrightKey; }}
		property Platform::String^ Copyright
		{
			Platform::String^ get() { return GetPropertyValue(copyrightKey); }
			void set(Platform::String^ value) { SetPropertyValue(copyrightKey, value); }
		}

		static property Platform::String^ DiscNumberKey { Platform::String^ get() { return discNumberKey; }}
		property unsigned int DiscNumber
		{
			unsigned int get() { return Converter::PlatformStringToUInt(GetPropertyValue(discNumberKey)); }
			void set(unsigned int value) { SetPropertyValue(discNumberKey, value); }
		}

		static property Platform::String^ GenreKey { Platform::String^ get() { return genreKey; }}
		property Platform::String^ Genre
		{
			Platform::String^ get() { return GetPropertyValue(genreKey); }
			void set(Platform::String^ value) { SetPropertyValue(genreKey, value); }
		}

		static property Platform::String^ TitleKey { Platform::String^ get() { return titleKey; }}
		property Platform::String^ Title
		{
			Platform::String^ get() { return GetPropertyValue(titleKey); }
			void set(Platform::String^ value) { SetPropertyValue(titleKey, value); }
		}

		static property Platform::String^ TrackNumberKey { Platform::String^ get() { return trackNumberKey; }}
		property unsigned int TrackNumber
		{
			unsigned int get() { return Converter::PlatformStringToUInt(GetPropertyValue(trackNumberKey)); }
			void set(unsigned int value) { SetPropertyValue(trackNumberKey, value); }
		}

		static property Platform::String^ YearKey { Platform::String^ get() { return yearKey; }}
		property unsigned int Year
		{
			unsigned int get() { return Converter::PlatformStringToUInt(GetPropertyValue(yearKey)); }
			void set(unsigned int value) { SetPropertyValue(yearKey, value); }
		}

		property Picture^ Image;

		property IPlatformPropertyMap^ Properties
		{
			IPlatformPropertyMap^ get() { return properties; }
		}

		Tag();

	internal:
		Tag(const TagLib::Tag& tag);
		void UpdateTag(TagLib::Tag& tag);

	private:
		static Platform::String^ albumKey;
		static Platform::String^ albumArtistKey;
		static Platform::String^ artistKey;
		static Platform::String^ commentKey;
		static Platform::String^ composerKey;
		static Platform::String^ copyrightKey;
		static Platform::String^ discNumberKey;
		static Platform::String^ genreKey;
		static Platform::String^ titleKey;
		static Platform::String^ trackNumberKey;
		static Platform::String^ yearKey;

		PlatformPropertyMap^ properties;

		Platform::String^ GetPropertyValue(Platform::String^ key);
		void SetPropertyValue(Platform::String^ key, Platform::String^ value);
		void SetPropertyValue(Platform::String^ key, unsigned int value);
	};
}