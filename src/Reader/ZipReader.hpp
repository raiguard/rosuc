#pragma once

#include <filesystem>

typedef struct mz_zip_file_s mz_zip_file, mz_zip_entry;

class ZipReader
{
public:
  ZipReader(const std::filesystem::path& path);
  ~ZipReader();

  void setPattern(const char* pattern);
  bool gotoFirstEntry();
  bool gotoNextEntry();
  std::string readEntry();

private:
  std::filesystem::path path;
  void* zip_reader;
  mz_zip_file* entryInfo;
};
