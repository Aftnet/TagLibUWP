$downloadedFileName = "taglib-src.zip"
wget "https://github.com/taglib/taglib/archive/taglib2.zip" -outfile $downloadedFileName

$extractDir = "Temp"
New-Item -ItemType directory $extractDir -Force
Expand-Archive -Path $downloadedFileName -DestinationPath $extractDir -Force

$srcDir = "Temp\\taglib-taglib2\\taglib"
$destDir = "Source"
Remove-Item $destDir -Recurse -Force
New-Item -ItemType directory -Path $destDir
Copy-Item ($srcDir+"\\*") -Destination $destDir -Recurse -Force

Remove-Item $extractDir -Recurse -Force
del $downloadedFileName