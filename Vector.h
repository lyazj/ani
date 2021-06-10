#pragma once

#include <stddef.h>

#include <iostream>

template<class T, size_t n>
struct Vector {
  T data[n];
  T &operator[](size_t i) { return data[i]; }
  const T &operator[](size_t i) const { return data[i]; }

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
