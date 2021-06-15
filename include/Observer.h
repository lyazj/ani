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
