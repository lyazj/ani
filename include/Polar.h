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

#include "Vector.h"

#include <math.h>
#include <assert.h>

template<class T, size_t n>
struct Polar
{
  static_assert(n > 0, "zero size of Polar forbiden");

  T data[n];
  T &operator[](size_t i) { return data[i]; }
  const T &operator[](size_t i) const { return data[i]; }

  Polar() = default;

  Polar(const Vector<T, n> &vec)
  {
    T r(data[0] = vec.mold());
    for(size_t i = 1; i < n; ++i)
      r *= sin(data[i] = acos(vec[i - 1] / r));
    if(vec[n - 1] < T(0))
      data[n - 1] *= T(-1);
  }

  operator Vector<T, n>() const
  {
    Vector<T, n> vec;
    T &r(vec[n - 1]);
    r = data[0];
    for(size_t i = 1; i < n; ++i)
    {
      vec[i - 1] = r * cos(data[i]);
      r *= sin(data[i]);
    }
    return vec;
  }
};

template<class T>
struct Polar<T, 3>
{
  T data[3];
  T &operator[](size_t i) { return data[i]; }
  const T &operator[](size_t i) const { return data[i]; }

  Polar() = default;

  Polar(const Vector<T, 3> &vec)
  {
    T r(data[0] = vec.mold());
    r *= sin(data[1] = acos(vec[2] / r));
    data[2] = acos(vec[0] / r);
    if(vec[1] < T(0))
      data[2] *= T(-1);
  }

  operator Vector<T, 3>() const
  {
    Vector<T, 3> vec;
    T &r(vec[1]);
    r = data[0];
    vec[2] = r * cos(data[1]);
    r *= sin(data[1]);
    vec[0] = r * cos(data[2]);
    r *= sin(data[2]);
    return vec;
  }
};

template<class T, size_t n> std::istream &
operator>>(std::istream &is, Polar<T, n> &v)
{
  for(size_t i = 0; i < n; ++i)
    is >> v[i];
  return is;
}

template<class T, size_t n> std::ostream &
operator<<(std::ostream &os, const Polar<T, n> &v)
{
  os << v[0];
  for(size_t i = 1; i < n; ++i)
    os << " " << v[i];
  return os;
}
