#include "Util.hpp"
#include "ZipReader.hpp"
#include <fmt/core.h>
#include <mz.h>
#include <mz_strm.h>
#include <mz_zip.h>
#include <mz_zip_rw.h>
#include <vector>

ZipReader::ZipReader(const std::filesystem::path& path)
  : path(path)
  , zip_reader(mz_zip_reader_create())
{
  if (int32_t err = mz_zip_reader_open_file(this->zip_reader, path.c_str()); err != MZ_OK)
    throw std::runtime_error(fmt::format("Failed to read zip file: error code {}", err));

}

ZipReader::~ZipReader()
{
  mz_zip_reader_close(this->zip_reader);
  mz_zip_reader_delete(&this->zip_reader);
}

void ZipReader::setPattern(const char* pattern)
{
  mz_zip_reader_set_pattern(this->zip_reader, pattern, 0);
}

void ZipReader::gotoFirstEntry()
{
  if (mz_zip_reader_goto_first_entry(this->zip_reader) == MZ_END_OF_LIST)
    throw std::runtime_error("Reached end of zip file");
  if (mz_zip_reader_entry_get_info(zip_reader, &this->entryInfo) == MZ_OK)
    fmt::println("Zip first entry: {}", this->entryInfo->filename);
}

void ZipReader::readEntry()
{
  if (!this->entryInfo)
    Util::panic("Attempted to read zip entry without entry being open");

  if (mz_zip_reader_entry_open(this->zip_reader) != MZ_OK)
    Util::panic("Failed to open zip entry: {}", this->entryInfo->filename);

  std::vector<char> buf;
  buf.resize(this->entryInfo->uncompressed_size);
  int32_t bytes_read = 0;
  bytes_read = mz_zip_reader_entry_read(this->zip_reader, buf.data(), buf.size());
  if (bytes_read < buf.size())
    Util::panic("Didn't read whole zip entry: expected {}, got {}", buf.size(), bytes_read);
  mz_zip_reader_entry_close(this->zip_reader);
  this->entryContent = std::string(buf.data());
}
