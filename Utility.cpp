#include "Utility.h"

#include <ctype.h>

#include <iterator>
#include <algorithm>
#include <sstream>

using namespace std;

string strip(const string &str)
{
  int (*is_valid_char)(int)(isalnum);
  auto b(find_if(str.begin(), str.end(), is_valid_char));
  auto e(find_if(str.rbegin(), make_reverse_iterator(b),
        is_valid_char).base());
  return string(b, e);
}

bool check_headline(const string &head)
{
  static const char labels[][10] {
    "no", "id", "name", "status",
    "mothers", "daughters", "colours",
    "p_x", "p_y", "p_z", "e", "m"
  };
  istringstream iss(head);
  string buf;
  for(const char *str : labels)
  {
    iss >> buf;
    if(buf != str)
      return false;
  }
  return true;
}
