# TagLibUWP

[![Build status](https://ci.appveyor.com/api/projects/status/4gd0dmwjwuvon82w?svg=true)](https://ci.appveyor.com/project/albertofustinoni/taglibuwp)

Port of [TagLib](http://taglib.org/) to UWP WinRT component. Uses WinRT's new file IO API to work with the sandboxed app model.
Right now, only basic tag data is exposed (Album, Artist, Genre, Title, Track number, Year).

TagLibUWP is distributed under the [Mozilla Public License](https://www.mozilla.org/media/MPL/2.0/index.815ca599c9df.txt)

## Installation

The recommended way to install TagLibUWP is via the [NuGet package](https://www.nuget.org/packages/TagLibUWP/)

Run the following command in the [Package Manager Console](https://docs.nuget.org/docs/start-here/using-the-package-manager-console):

```
PM> Install-Package TagLibUWP 
```

## Usage

```csharp
IStorageFile file = GetFromWherever();

//Read tag from file
//IO operations are done synchronously, never use TagManager from the UI thread.
var fileInfo = await Task.Run(() => TagManager.ReadFile(file));
var tag = fileInfo.Tag;
string Album = tag.Album; //And so forth for other tag data

//Modify and save tags
tag.Album = "NewAlbum";
await Task.Run(() => TagManager.WriteFile(file, tag));
```
