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

#include <utility>
#include <algorithm>

template<class T>
T correct(T t, T t_min = T(-1), T t_max = T(1))
{
  return std::min(std::max(t, t_min), t_max);
}

template<class T, size_t n>
struct Polar;

template<class T, size_t n>
struct Direction {
  static_assert(n > 0, "zero size of Direction forbiden");

  T data[n - 1];
  T &operator[](size_t i)
  {
    return i == 0 ? (_r = T(1)) : data[i - 1];
  }
  const T &operator[](size_t i) const
  {
    return i == 0 ? (_r = T(1)) : data[i - 1];
  }

  static T _r;

  operator Polar<T, n>() const
  {
    Polar<T, n> pol;
    pol[0] = T(1);
    for(size_t i = 1; i < n; ++i)
      pol[i] = data[i - 1];
    return pol;
  }

  Direction &operator+=(const Direction &rhs)
  {
    for(size_t i = 0; i < n - 1; ++i)
      data[i] += rhs.data[i];
    return *this;
  }
  Direction &operator-=(const Direction &rhs)
  {
    for(size_t i = 0; i < n - 1; ++i)
      data[i] -= rhs.data[i];
    return *this;
  }
};

template<class T, size_t n>
T Direction<T, n>::_r = T(1);

template<class T, size_t n> Direction<T, n>
operator+(Direction<T, n> lhs, const Direction<T, n> &rhs)
{
  return lhs += rhs;
}

template<class T, size_t n> Direction<T, n>
operator-(Direction<T, n> lhs, const Direction<T, n> &rhs)
{
  return lhs -= rhs;
}

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
      r *= sin(data[i] = (r == T(0) ?
            T(0) : acos(correct(vec[i - 1] / r))));
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

  Direction<T, n> direction() const
  {
    Direction<T, n> dir;
    for(size_t i = 1; i < n; ++i)
      dir[i] = data[i];
    return dir;
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
    r *= sin(data[1] = (r == T(0) ?
          T(0) : acos(correct(vec[2] / r))));
    data[2] = (r == 0 ? T(0) : acos(correct(vec[0] / r)));
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

  Direction<T, 3> direction() const
  {
    Direction<T, 3> dir;
    for(size_t i = 1; i < 3; ++i)
      dir[i] = data[i];
    return dir;
  }
};

template<class T, size_t n>
Vector<T, n> translate(Vector<T, n> r, const Vector<T, n> &r0)
{
  return r - r0;
}

template<class T, size_t n>
Polar<T, n> rotate(Polar<T, n> p, const Direction<T, n> &d)
{
  for(size_t i = 1; i < n; ++i)
    p[i] -= d[i];
  return p;
}

template<class T, size_t n>
Vector<T, n> rotate(const Vector<T, n> &r, const Direction<T, n> &d)
{
  return rotate(Polar<T, n>(r), d);
}

template<class T, size_t n> std::istream &
operator>>(std::istream &is, Direction<T, n> &v)
{
  for(size_t i = 1; i < n; ++i)
    is >> v[i];
  return is;
}

template<class T, size_t n> std::ostream &
operator<<(std::ostream &os, const Direction<T, n> &v)
{
  os << v[1];
  for(size_t i = 2; i < n; ++i)
    os << " " << v[i];
  return os;
}
template<class T> std::ostream &
operator<<(std::ostream &os, const Direction<T, 1> &v)
{
  return os;
}

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

constexpr double radians(double deg)
{
  return deg * M_PI / 180;
}

constexpr double degrees(double rad)
{
  return rad / M_PI * 180;
}
