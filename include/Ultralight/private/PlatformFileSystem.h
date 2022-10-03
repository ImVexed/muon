#pragma once
#include <Ultralight/Defines.h>
#include <Ultralight/String.h>

namespace ultralight {
namespace PlatformFileSystem {

bool UExport FileExists(const String& path);
bool UExport IsDirectory(const String& path, bool follow_symbolic_links);
String UExport AppendPath(const String& path, const String& component);
bool UExport MakeAllDirectories(const String& path);
String UExport GetFileName(const String& path);
String UExport GetDirectoryName(const String& path);
String UExport RealFilePath(const String& path);

}  // PlatformFileSystem
}  // ultralight
