#include "dataStruct.hpp"
#include <limits>

std::pair<std::string, std::string> ankhbayar::getNextPair(std::string& s)
{
  s.erase(0, s.find(':')+1);
  std::string key=s.substr(0, 4);
  s.erase(0,5);
  std::string value;
  if(key=="key3")
  {
    value=s.substr(0, s.find('"', 1)+1);
    s.erase(0, value.size());
  }
  else
  {
    value=s.substr(0, s.find(':'));
    s.erase(0, value.size());
  }
  return std::make_pair(key, value);
}
std::istream& ankhbayar::operator>>(std::istream& in, ankhbayar::DataStruct& ds)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string token;
  getline(in, token);
  while(!token.empty())
  {
    std::pair<std::string, std::string> pair=ankhbayar::getNextPair(token);
    if (pair.first=="key1")
    {
      if (!ankhbayar::isULL_OCT(pair.second))
      {
        in.setstate(std::ios::badbit);
        return in;
      }
      ds.key1=std::strtoull(pair.second.c_str(), nullptr, 8);
    }
    else if (pair.first=="key2")
    {
      if (!ankhbayar::isULL_HEX(pair.second)) {
        in.setstate(std::ios::badbit);
        return in;
      }
      ds.key2=std::strtoull(pair.second.c_str(), nullptr, 16);
    }
    else if (pair.first=="key3")
    {
      if(!ankhbayar::isString(pair.second))
      {
        in.setstate(std::ios::badbit);
        return in;
      }
      ds.key3=pair.second.substr(1, pair.second.size() -2);
    }
  }
  return in;
}


std::ostream& ankhbayar::operator<<(std::ostream& out, const ankhbayar::DataStruct& ds)
{
  std::ostream::sentry sentry(out);
  if(!sentry)
  {
    return out;
  }
  iofmtguard fmtguard(out);
  out << "(:key1 0" << std::oct << ds.key1 << ":key2 0x" << std::uppercase << std::hex << ds.key2 << ":key3\"" << ds.key3 << "\":)";
  return out;
}

bool ankhbayar::isULL_OCT(const std::string& str)
{
  const std::regex regular("^0[0-7]+$");
  return std::regex_match(str, regular);
}
bool ankhbayar::isULL_HEX(const std::string& str)
{
  const std::regex regular("^0[xX][0-9A-Fa-f]+$");
  return std::regex_match(str, regular);
}
bool ankhbayar::isString(const std::string& str)
{
  const std::regex regular("\".*\"");
  return std::regex_match(str, regular);
}
ankhbayar::iofmtguard::iofmtguard(std::basic_ios< char >& s):
  s_(s),
  fill_(s.fill()),
  precision_(s.precision()),
  fmt_(s.flags())
{}
ankhbayar::iofmtguard::~iofmtguard()
{
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
