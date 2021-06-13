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
#include "System.h"

#include <fstream>
#include <memory>

using namespace std;

int simulate(istream &is, ostream &os, int prolong)
{
  System system;
  system.prolong = prolong > 0 ? prolong : 0;
  return !(is >> system) || !(os << system);
}

int simulate(const string &infile, const string &outfile, int prolong)
{
  ifstream ifs;
  ofstream ofs;
  return simulate(infile == "-" ? cin : (ifs.open(infile), ifs),
      outfile == "-" ? cout : (ofs.open(outfile), ofs), prolong);
}

extern "C" {

int simulate(const char *infile, const char *outfile)
{
  return simulate(infile, outfile, 0);
}

int simulate_prolong(const char *infile, const char *outfile, int prolong)
{
  return simulate(infile, outfile, prolong);
}

}
