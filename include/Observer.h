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

#include "Coordinate.h"

struct Observer {
  Vector<double, 3> r0;
  Direction<double, 3> d0;

  Vector<double, 3> observe(const Vector<double, 3> &r)
  {
    return rotate(translate(r, r0), d0 - d_norm);
  }

  static constexpr Direction<double, 3> d_norm = {
    radians(90), radians(0)
  };
};
