#pragma once

#include "Picture.h"

#include "tag.h"
#include "toolkit\tpropertymap.h"

#include <collection.h>

namespace TagLibUWP
{
	typedef Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^> IPlatformPropertyMap;
	typedef Platform::Collections::Map<Platform::String^, Platform::String^> PlatformPropertyMap;

	public ref class Tag sealed
	{
	public:
		static property Platform::String^ AlbumKey { Platform::String^ get() { return albumKey; }}
		property Platform::String^ Album
		{
			Platform::String^ get() { return properties->Lookup(albumKey); }
			void set(Platform::String^ value) { properties->Insert(albumKey, value); }
		}

		static property Platform::String^ AlbumArtistKey { Platform::String^ get() { return albumArtistKey; }}
		property Platform::String^ AlbumArtist
		{
			Platform::String^ get() { return properties->Lookup(albumArtistKey); }
			void set(Platform::String^ value) { properties->Insert(albumArtistKey, value); }
		}

		static property Platform::String^ ArtistKey { Platform::String^ get() { return artistKey; }}
		property Platform::String^ Artist
		{
			Platform::String^ get() { return properties->Lookup(artistKey); }
			void set(Platform::String^ value) { properties->Insert(artistKey, value); }
		}

		static property Platform::String^ CommentKey { Platform::String^ get() { return commentKey; }}
		property Platform::String^ Comment
		{
			Platform::String^ get() { return properties->Lookup(commentKey); }
			void set(Platform::String^ value) { properties->Insert(commentKey, value); }
		}

		static property Platform::String^ ComposerKey { Platform::String^ get() { return composerKey; }}
		property Platform::String^ Composer
		{
			Platform::String^ get() { return properties->Lookup(composerKey); }
			void set(Platform::String^ value) { properties->Insert(composerKey, value); }
		}

		static property Platform::String^ CopyrightKey { Platform::String^ get() { return copyrightKey; }}
		property Platform::String^ Copyright
		{
			Platform::String^ get() { return properties->Lookup(copyrightKey); }
			void set(Platform::String^ value) { properties->Insert(copyrightKey, value); }
		}

		static property Platform::String^ DiscNumberKey { Platform::String^ get() { return discNumberKey; }}
		property unsigned int DiscNumber
		{
			unsigned int get() { return PlatformStringToUInt(properties->Lookup(discNumberKey)); }
			void set(unsigned int value) { properties->Insert(discNumberKey, UIntToPlatformString(value)); }
		}

		static property Platform::String^ GenreKey { Platform::String^ get() { return genreKey; }}
		property Platform::String^ Genre
		{
			Platform::String^ get() { return properties->Lookup(genreKey); }
			void set(Platform::String^ value) { properties->Insert(genreKey, value); }
		}

		static property Platform::String^ TitleKey { Platform::String^ get() { return titleKey; }}
		property Platform::String^ Title
		{
			Platform::String^ get() { return properties->Lookup(titleKey); }
			void set(Platform::String^ value) { properties->Insert(titleKey, value); }
		}

		static property Platform::String^ TrackKey { Platform::String^ get() { return trackKey; }}
		property unsigned int Track
		{
			unsigned int get() { return PlatformStringToUInt(properties->Lookup(trackKey)); }
			void set(unsigned int value) { properties->Insert(trackKey, UIntToPlatformString(value)); }
		}

		static property Platform::String^ YearKey { Platform::String^ get() { return yearKey; }}
		property unsigned int Year
		{
			unsigned int get() { return PlatformStringToUInt(properties->Lookup(yearKey)); }
			void set(unsigned int value) { properties->Insert(yearKey, UIntToPlatformString(value)); }
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
		static Platform::String^ trackKey;
		static Platform::String^ yearKey;

		PlatformPropertyMap^ properties;

		TagLib::String PlatformToTagLibString(Platform::String^ value);
		Platform::String^ TagLibToPlatformString(const TagLib::String& value);

		TagLib::PictureMap PictureToPictureMap(Picture^ value);

		TagLib::SimplePropertyMap PlatformToTagLibMap(PlatformPropertyMap^ value);
		PlatformPropertyMap^ TagLibToPlatformMap(const TagLib::SimplePropertyMap& value);

		Platform::String^ UIntToPlatformString(unsigned int value);
		unsigned int PlatformStringToUInt(Platform::String^ value);
	};
}