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

            Assert.Equal(nameof(tag.Album), tag.Album);
            Assert.Equal(nameof(tag.Album), tagProperties["ALBUM"]);
            Assert.Equal(nameof(tag.Artist), tag.Artist);
            Assert.Equal(nameof(tag.Artist), tagProperties["ARTIST"]);
            Assert.Equal(nameof(tag.Comment), tag.Comment);
            Assert.Equal(nameof(tag.Comment), tagProperties["COMMENT"]);
            Assert.Equal(nameof(tag.Genre), tag.Genre);
            Assert.Equal(nameof(tag.Genre), tagProperties["GENRE"]);
            var titleRef = nameof(tag.Title) + "あア亜";
            Assert.Equal(titleRef, tag.Title);
            Assert.Equal(titleRef, tagProperties["TITLE"]);
            Assert.Equal(45U, tag.Track);
            Assert.Equal("45", tagProperties["TRACKNUMBER"]);
            Assert.Equal(2000U, tag.Year);
            Assert.Equal("2000", tagProperties["DATE"]);

            Assert.Equal("AlbumArtist", tagProperties["ALBUMARTIST"]);
            Assert.Equal("Composer", tagProperties["COMPOSER"]);
            Assert.Equal("67", tagProperties["DISCNUMBER"]);
            Assert.Equal("Copyright", tagProperties["COPYRIGHT"]);

            //Assert.Equal("Custom", tagProperties["CUSTOM"]);
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
        public Task EmptyTagWritingWorks(string fileName)
        {
            return TagWritingTest(fileName, d => string.Empty);
        }

        public async Task TagWritingTest(string fileName, Func<string, string> TagTransformation)
        {
            var file = await GetTestMediaFileAsync(fileName);
            file = await CopyToTempFileAsync(file);

            var fileInfo = await Task.Run(() => TagManager.ReadFile(file));

            var tag = fileInfo.Tag;
            tag.Album = TagTransformation(nameof(tag.Album));
            tag.Artist = TagTransformation(nameof(tag.Artist));
            tag.Genre = TagTransformation(nameof(tag.Genre));
            tag.Title = TagTransformation(nameof(tag.Title) + "あア亜");
            tag.Comment = TagTransformation(nameof(tag.Comment));
            var newTrackNumber = 23U;
            tag.Track = newTrackNumber;
            var newYear = 2096U;
            tag.Year = newYear;

            await Task.Run(() => TagManager.WriteFile(file, tag));

            fileInfo = await Task.Run(() => TagManager.ReadFile(file));
            tag = fileInfo.Tag;
            Assert.Equal(TagTransformation(nameof(tag.Album)), tag.Album);
            Assert.Equal(TagTransformation(nameof(tag.Artist)), tag.Artist);
            Assert.Equal(TagTransformation(nameof(tag.Genre)), tag.Genre);
            Assert.Equal(TagTransformation(nameof(tag.Title) + "あア亜"), tag.Title);
            Assert.Equal(TagTransformation(nameof(tag.Comment)), tag.Comment);
            Assert.Equal(newTrackNumber, tag.Track);
            Assert.Equal(newYear, tag.Year);

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
    }
}
