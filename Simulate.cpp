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

#include "Interface.h"

#include <iostream>

#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
  cin.sync_with_stdio(0);
  if(argc > 4)
  {
    cerr << "ERROR: Wrong number of arguments." << endl;
    return 1;
  }
  const char *infile(argc > 1 ? (const char *)argv[1] : "-");
  const char *outfile(argc > 2 ? (const char *)argv[2] : "-");
  int prolong(argc > 3 ? atoi(argv[3]) : 0);
  return simulate(infile, outfile, prolong);
}
