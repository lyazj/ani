#pragma once

#include "Vector.h"

#include <stddef.h>

#include <string>
#include <sstream>
#include <set>
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
  
  Vector<double, 3> getv() const
  {
    return p / e;
  }
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

struct { } hdrp;
struct { } endp;

class Particle_Printer {
public:
  Particle_Printer(std::ostream &s,
      size_t no, size_t name, size_t e, size_t ph)
    : os(s), no_width(no),
      name_width(name), e_width(e), phase_width(ph) { }
  Particle_Printer &operator<<(const Particle &p);
  Particle_Printer &operator<<(const decltype(hdrp) &)
  {
    print_header();
    return *this;
  }
  std::ostream &operator<<(const decltype(endp) &)
  {
    return os << std::endl;
  }
private:
  std::ostream &os;
  size_t no_width, name_width, e_width, phase_width;
  void print_header();
};
