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

#include <stddef.h>

#include <set>
#include <string>
#include <sstream>
#include <iostream>

#define PHASE_UNDEF ((size_t)-1)
#define PHASE_MIN   ((size_t) 0)

struct Particle_Base {
  size_t no;
  int id;
  std::string name;
  int status;
  size_t mothers[2];
  size_t daughters[2];
  short colours[2];
  Vector<double, 3> p;
  double e;
  double m;
  
  Vector<double, 3> getv() const { return p / e; }
  virtual ~Particle_Base() { }
};

struct Particle : public Particle_Base {
  std::set<size_t> mothers;
  std::set<size_t> daughters;
  Vector<double, 3> r;
  Vector<double, 3> v;
  size_t phase;
  size_t death;
};

std::istream &operator>>(std::istream &is, Particle_Base &pb);
std::istream &operator>>(std::istream &is, Particle &p);

constexpr struct { } hdrp = { };
constexpr struct { } endp = { };

class Particle_Printer {
public:
  Particle_Printer(std::ostream &s, size_t no,
      size_t name, size_t e, size_t ph)
    : os(s), no_width(no),
      name_width(name), e_width(e), phase_width(ph) { }
  Particle_Printer &operator<<(const Particle &p);
  Particle_Printer &operator<<(const decltype(hdrp) &)
  {
    print_header();
    return *this;
  }
  Particle_Printer &operator<<(const decltype(endp) &)
  {
    os << std::endl;
    return *this;
  }
private:
  std::ostream &os;
  size_t no_width, name_width, e_width, phase_width;
  void print_header();
};
