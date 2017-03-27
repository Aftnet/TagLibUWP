using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using Windows.ApplicationModel;
using Windows.Storage;
using Xunit;

namespace TagLibUWP.Test
{
    public class TagManagerTest
    {
        private static string[] UnsupportedAudioExtensions = new string[] { "aac", "txt" };
        public static IEnumerable<object[]> UnsupportedAudioFileNames { get { return UnsupportedAudioExtensions.Select(d => new object[] { "Unsupported." + d }); } }

        private static string[] SupportedAudioExtensions = new string[] { "asf", "flac", "m4a", "m4v", "mp3", "mp4", "ogg", "wma", "wav", "wv" };
        public static IEnumerable<object[]> SupportedAudioFileNames { get { return SupportedAudioExtensions.Select(d => new object[] { "AudioTest." + d }); } }

        private const string JpegMimeType = "image/jpeg";
        private const string PNGMimeType = "image/png";

        private static string[] TestPictureNames = new string[] { "Cover01.jpg", "Cover02.jpg" };

        [Fact(DisplayName = "Getting supported extensions")]
        public void GetSupportedExtensionsWorks()
        {
            var extensions = TagManager.SupportedExtensions;
            Assert.NotNull(extensions);
            Assert.NotEmpty(extensions);
        }

        [Theory(DisplayName = "Unsupported files handling"), MemberData(nameof(UnsupportedAudioFileNames))]
        public async Task UnsupportedFilesHandlingWorks(string fileName)
        {
            var file = await GetTestMediaFileAsync(fileName);

            try
            {
                var tag = TagManager.ReadFile(file);
                Assert.True(false);
            }
            catch(COMException e)
            {
                Assert.NotNull(e);
            }
        }

        [Theory(DisplayName = "Tag reading"), MemberData(nameof(SupportedAudioFileNames))]
        public async Task TagReadingWorks(string fileName)
        {
            var file = await GetTestMediaFileAsync(fileName);

            var fileInfo = await Task.Run(() => TagManager.ReadFile(file));
            Assert.Same(file, fileInfo.File);
            Assert.NotNull(fileInfo.Properties);
            
            var tag = fileInfo.Tag;
            var tagProperties = tag.Properties;
            Assert.NotNull(tag);
            Assert.NotNull(tag.Properties);

            AssertTagEqual(nameof(Tag.Album), tag.Album, tagProperties, Tag.AlbumKey);
            AssertTagEqual(nameof(Tag.AlbumArtist), tag.AlbumArtist, tagProperties, Tag.AlbumArtistKey);
            AssertTagEqual(nameof(Tag.Artist), tag.Artist, tagProperties, Tag.ArtistKey);
            AssertTagEqual(nameof(Tag.Comment), tag.Comment, tagProperties, Tag.CommentKey);
            AssertTagEqual(nameof(Tag.Composer), tag.Composer, tagProperties, Tag.ComposerKey);
            AssertTagEqual(nameof(Tag.Copyright), tag.Copyright, tagProperties, Tag.CopyrightKey);
            AssertTagEqual(67U, tag.DiscNumber, tagProperties, Tag.DiscNumberKey);
            AssertTagEqual(nameof(Tag.Genre), tag.Genre, tagProperties, Tag.GenreKey);
            AssertTagEqual(nameof(Tag.Title) + "あア亜", tag.Title, tagProperties, Tag.TitleKey);
            AssertTagEqual(45U, tag.TrackNumber, tagProperties, Tag.TrackNumberKey);
            AssertTagEqual(2000U, tag.Year, tagProperties, Tag.YearKey);
        }

        [Theory(DisplayName = "Longer tag writing"), MemberData(nameof(SupportedAudioFileNames))]
        public Task LongerTagWritingWorks(string fileName)
        {
            return TagWritingTest(fileName, d => d + "-suffix");
        }

        [Theory(DisplayName = "Same length tag writing"), MemberData(nameof(SupportedAudioFileNames))]
        public Task SameLengthTagWritingWorks(string fileName)
        {
            return TagWritingTest(fileName, d => d.Substring(2) + "zz");
        }

        [Theory(DisplayName = "Shorter tag writing"), MemberData(nameof(SupportedAudioFileNames))]
        public Task ShorterTagWritingWorks(string fileName)
        {
            return TagWritingTest(fileName, d => d.Substring(2));
        }

        [Theory(DisplayName = "Empty tag writing"), MemberData(nameof(SupportedAudioFileNames))]
        public async Task EmptyTagWritingWorks(string fileName)
        {
            var file = await GetTestMediaFileAsync(fileName);
            file = await CopyToTempFileAsync(file);

            var fileInfo = await Task.Run(() => TagManager.ReadFile(file));
            var tag = fileInfo.Tag;

            tag.Album = string.Empty;
            Assert.False(tag.Properties.ContainsKey(nameof(tag.Album)));
            tag.AlbumArtist = string.Empty;
            Assert.False(tag.Properties.ContainsKey(nameof(tag.AlbumArtist)));
            tag.Artist = string.Empty;
            Assert.False(tag.Properties.ContainsKey(nameof(tag.Artist)));
            tag.Comment = string.Empty;
            Assert.False(tag.Properties.ContainsKey(nameof(tag.Comment)));
            tag.Composer = string.Empty;
            Assert.False(tag.Properties.ContainsKey(nameof(tag.Composer)));
            tag.Copyright = string.Empty;
            Assert.False(tag.Properties.ContainsKey(nameof(tag.Copyright)));
            tag.DiscNumber = 0;
            Assert.False(tag.Properties.ContainsKey(nameof(tag.DiscNumber)));
            tag.Genre = string.Empty;
            Assert.False(tag.Properties.ContainsKey(nameof(tag.Genre)));
            tag.Title = string.Empty;
            Assert.False(tag.Properties.ContainsKey(nameof(tag.Title)));
            tag.Comment = string.Empty;
            Assert.False(tag.Properties.ContainsKey(nameof(tag.Comment)));
            tag.TrackNumber = 0;
            Assert.False(tag.Properties.ContainsKey(nameof(tag.TrackNumber)));
            tag.Year = 0;
            Assert.False(tag.Properties.ContainsKey(nameof(tag.Year)));

            tag.Properties.Clear();
            await Task.Run(() => TagManager.WriteFile(file, tag));

            fileInfo = await Task.Run(() => TagManager.ReadFile(file));
            tag = fileInfo.Tag;
            Assert.Empty(tag.Properties);
        }

        public async Task TagWritingTest(string fileName, Func<string, string> TagTransformation)
        {
            var file = await GetTestMediaFileAsync(fileName);
            file = await CopyToTempFileAsync(file);

            var fileInfo = await Task.Run(() => TagManager.ReadFile(file));
            var tag = fileInfo.Tag;

            tag.Album = TagTransformation(nameof(tag.Album));
            tag.AlbumArtist = TagTransformation(nameof(tag.AlbumArtist));
            tag.Artist = TagTransformation(nameof(tag.Artist));
            tag.Comment = TagTransformation(nameof(tag.Comment));
            tag.Composer = TagTransformation(nameof(tag.Composer));
            tag.Copyright = TagTransformation(nameof(tag.Copyright));
            var newDiscNumber = 113U;
            tag.DiscNumber = newDiscNumber;
            tag.Genre = TagTransformation(nameof(tag.Genre));
            var sampleTitle = nameof(tag.Title) + "あア亜";
            tag.Title = TagTransformation(sampleTitle);
            tag.Comment = TagTransformation(nameof(tag.Comment));
            var newTrackNumber = 23U;
            tag.TrackNumber = newTrackNumber;
            var newYear = 2096U;
            tag.Year = newYear;

            await Task.Run(() => TagManager.WriteFile(file, tag));

            fileInfo = await Task.Run(() => TagManager.ReadFile(file));
            tag = fileInfo.Tag;
            var tagProperties = tag.Properties;

            AssertTagEqual(TagTransformation(nameof(Tag.Album)), tag.Album, tagProperties, Tag.AlbumKey);
            AssertTagEqual(TagTransformation(nameof(Tag.AlbumArtist)), tag.AlbumArtist, tagProperties, Tag.AlbumArtistKey);
            AssertTagEqual(TagTransformation(nameof(Tag.Artist)), tag.Artist, tagProperties, Tag.ArtistKey);
            AssertTagEqual(TagTransformation(nameof(Tag.Comment)), tag.Comment, tagProperties, Tag.CommentKey);
            AssertTagEqual(TagTransformation(nameof(Tag.Composer)), tag.Composer, tagProperties, Tag.ComposerKey);
            AssertTagEqual(TagTransformation(nameof(Tag.Copyright)), tag.Copyright, tagProperties, Tag.CopyrightKey);
            AssertTagEqual(newDiscNumber, tag.DiscNumber, tagProperties, Tag.DiscNumberKey);
            AssertTagEqual(TagTransformation(nameof(Tag.Genre)), tag.Genre, tagProperties, Tag.GenreKey);
            AssertTagEqual(TagTransformation(sampleTitle), tag.Title, tagProperties, Tag.TitleKey);
            AssertTagEqual(newTrackNumber, tag.TrackNumber, tagProperties, Tag.TrackNumberKey);
            AssertTagEqual(newYear, tag.Year, tagProperties, Tag.YearKey);

            await file.DeleteAsync();
        }

        [Theory(DisplayName = "Image reading"), MemberData(nameof(SupportedAudioFileNames))]
        public async Task ImageReadingWorks(string fileName)
        {
            var file = await GetTestMediaFileAsync(fileName);

            var fileInfo = await Task.Run(() => TagManager.ReadFile(file));

            var image = fileInfo.Tag.Image;
            Assert.NotNull(image);
            Assert.Equal(JpegMimeType, image.MIMEType);
            Assert.NotNull(image.Data);
            Assert.Equal(151308, image.Data.Length);
        }

        [Theory(DisplayName = "Image update"), MemberData(nameof(SupportedAudioFileNames))]
        public async Task ImageUpdateWorks(string fileName)
        {
            var file = await GetTestMediaFileAsync(fileName);
            file = await CopyToTempFileAsync(file);

            var fileInfo = await Task.Run(() => TagManager.ReadFile(file));

            var testImageMIME = PNGMimeType;
            var testImageBytes = new byte[] { 1, 2, 3, 4, 5 };

            var image = fileInfo.Tag.Image;
            image.MIMEType = testImageMIME;
            image.Data = testImageBytes;

            await Task.Run(() => TagManager.WriteFile(file, fileInfo.Tag));

            fileInfo = await Task.Run(() => TagManager.ReadFile(file));
            image = fileInfo.Tag.Image;

            Assert.Equal(testImageMIME, image.MIMEType);
            Assert.Equal(testImageBytes.Length, image.Data.Length);
            for (var i = 0; i < testImageBytes.Length; i++)
            {
                Assert.Equal(testImageBytes[i], image.Data[i]);
            }
        }

        [Theory(DisplayName = "Image removal"), MemberData(nameof(SupportedAudioFileNames))]
        public async Task ImageRemovalWorks(string fileName)
        {
            var file = await GetTestMediaFileAsync(fileName);
            file = await CopyToTempFileAsync(file);

            var fileInfo = await Task.Run(() => TagManager.ReadFile(file));

            fileInfo.Tag.Image = null;
            await Task.Run(() => TagManager.WriteFile(file, fileInfo.Tag));

            fileInfo = await Task.Run(() => TagManager.ReadFile(file));
            Assert.Null(fileInfo.Tag.Image);
        }

        [Fact(DisplayName = "Cover update performance")]
        public async Task ImageReplacementPerformanceTest()
        {
            var file = await GetTestMediaFileAsync("AudioTest.mp3");
            file = await CopyToTempFileAsync(file);

            var testPictureBytes = new byte[TestPictureNames.Length][];
            for (var i = 0; i < testPictureBytes.Length; i++)
            {
                testPictureBytes[i] = await LoadFileToMemoryAsync(TestPictureNames[i]);
            }

            var totalTime = await Task.Run(() =>
            {
                var output = TimeSpan.Zero;

                var numIterations = 300;
                for (var i = 0; i < numIterations; i++)
                {
                    var index = i % testPictureBytes.Length;
                    var fileInfo = TagManager.ReadFile(file);
                    fileInfo.Tag.Image = new Picture
                    {
                        Data = testPictureBytes[index],
                        MIMEType = JpegMimeType
                    };
                    var startTime = DateTimeOffset.UtcNow;
                    TagManager.WriteFile(fileInfo);
                    var timeDelta = DateTimeOffset.UtcNow - startTime;
                    output += timeDelta;
                }

                return output;
            });

            Assert.True(totalTime.TotalSeconds < 10);
        }

        private async Task<IStorageFile> GetTestMediaFileAsync(string fileName)
        {
            var mediaFolderPath = Path.Combine(Package.Current.InstalledLocation.Path, "TestMedia");
            var mediaFolder = await StorageFolder.GetFolderFromPathAsync(mediaFolderPath);
            var file = await mediaFolder.GetFileAsync(fileName);
            return file;
        }

        private async Task<IStorageFile> CopyToTempFileAsync(IStorageFile input)
        {
            var folder = ApplicationData.Current.TemporaryFolder;
            var output = await input.CopyAsync(folder, "Temp-" + input.Name, NameCollisionOption.ReplaceExisting);
            return output;
        }

        private async Task<byte[]> LoadFileToMemoryAsync(string fileName)
        {
            var file = await GetTestMediaFileAsync(fileName);
            var stream = (await file.OpenReadAsync()).AsStreamForRead();
            var output = new byte[stream.Length];
            await stream.ReadAsync(output, 0, output.Length);
            return output;
        }

        private static void AssertTagEqual(string expectedValue, string tag, IDictionary<string, string> propertiesDictionary, string tagKey)
        {
            Assert.Equal(expectedValue, tag);
            if (!string.IsNullOrEmpty(expectedValue))
            {
                Assert.Equal(expectedValue, propertiesDictionary[tagKey]);
            }
            else
            {
                Assert.False(propertiesDictionary.ContainsKey(tagKey));
            }
        }

        private static void AssertTagEqual(uint expectedValue, uint tag, IDictionary<string, string> propertiesDictionary, string tagKey)
        {
            Assert.Equal(expectedValue, tag);
            Assert.Equal(expectedValue, uint.Parse(propertiesDictionary[tagKey]));
        }
    }
}
