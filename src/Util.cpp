#include "Util.hpp"
#include <cstring>
#include <fstream>
#include <string>

std::string Util::readFile(const std::filesystem::path& path)
{
  std::ifstream file(path);
  if (file.fail())
    Util::panic("Failed to open file: {}", strerror(errno));

  std::string content;
  file >> content;
  return content;
}
