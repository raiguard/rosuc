#include "Util.hpp"
#include <cstring>
#include <fstream>
#include <string>

std::string Util::readFile(const std::filesystem::path& path)
{
  std::ifstream file(path);
  if (file.fail())
    Util::panic("Failed to open file: {}", strerror(errno));

  std::stringstream content;
  content << file.rdbuf();
  return content.str();
}

void Util::trimString(std::string& s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) { return !std::isspace(ch); }));
  s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) { return !std::isspace(ch); }).base(), s.end());
}

bool ichar_equals(char a, char b)
{
  return std::tolower(static_cast<unsigned char>(a)) ==
         std::tolower(static_cast<unsigned char>(b));
}

bool Util::iequals(std::string_view lhs, std::string_view rhs)
{
  return std::ranges::equal(lhs, rhs, ichar_equals);
}
