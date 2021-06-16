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

#include <stddef.h>
#include <math.h>

#include <iostream>

template<class T, size_t n>
struct Vector {
  T data[n];
  T &operator[](size_t i) { return data[i]; }
  const T &operator[](size_t i) const { return data[i]; }

  Vector operator+() const
  {
    return *this;
  }
  Vector operator-() const
  {
    return *this * T(-1);
  }
  Vector &operator+=(const Vector &rhs)
  {
    for(size_t i = 0; i < n; ++i)
      data[i] += rhs.data[i];
    return *this;
  }
  Vector &operator-=(const Vector &rhs)
  {
    for(size_t i = 0; i < n; ++i)
      data[i] -= rhs.data[i];
    return *this;
  }
  Vector &operator*=(T t)
  {
    for(size_t i = 0; i < n; ++i)
      data[i] *= t;
    return *this;
  }
  Vector &operator/=(T t)
  {
    for(size_t i = 0; i < n; ++i)
      data[i] /= t;
    return *this;
  }

  T mold() const
  {
    T rst(0);
    for(size_t i = 0; i < n; ++i)
      rst += pow(data[i], 2);
    return pow(rst, 0.5);
  }
};

template<class T, size_t n> Vector<T, n>
operator+(Vector<T, n> lhs, const Vector<T, n> &rhs)
{
  return lhs += rhs;
}

template<class T, size_t n> Vector<T, n>
operator-(Vector<T, n> lhs, const Vector<T, n> &rhs)
{
  return lhs -= rhs;
}

template<class T, size_t n> Vector<T, n>
operator*(Vector<T, n> lhs, T t)
{
  return lhs *= t;
}
template<class T, size_t n> Vector<T, n>
operator*(T t, Vector<T, n> rhs)
{
  return rhs *= t;
}

template<class T, size_t n> Vector<T, n>
operator/(Vector<T, n> lhs, T t)
{
  return lhs /= t;
}

template<class T, size_t n> std::istream &
operator>>(std::istream &is, Vector<T, n> &v)
{
  for(size_t i = 0; i < n; ++i)
    is >> v[i];
  return is;
}

template<class T, size_t n> std::ostream &
operator<<(std::ostream &os, const Vector<T, n> &v)
{
  os << v[0];
  for(size_t i = 1; i < n; ++i)
    os << " " << v[i];
  return os;
}
template<class T> std::ostream &
operator<<(std::ostream &os, const Vector<T, 0> &v)
{
  return os;
}
