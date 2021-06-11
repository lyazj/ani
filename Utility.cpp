/*  Copyright (C) 2021 @lyazj
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
