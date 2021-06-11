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

#pragma once

#ifdef __cplusplus

#include <string>

int simulate(std::istream &is, std::ostream &os,
    int prolong = 0);
int simulate(const std::string &infile, const std::string &outfile,
    int prolong = 0);

extern "C" {

#endif

int simulate(const char *infile, const char *outfile);
int simulate_prolong(const char *infile, const char *outfile, int prolong);

#ifdef __cplusplus

}

#endif
