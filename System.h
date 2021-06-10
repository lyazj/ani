#pragma once

#include "Particle.h"

#include <string>
#include <vector>

class System;

std::istream &operator>>(std::istream &is, System &s);
std::ostream &operator<<(std::ostream &os, System &s);

class System {
  friend std::istream &operator>>(std::istream &, System &);
  friend std::ostream &operator<<(std::ostream &, System &);

public:
  std::string description;
  std::vector<Particle> hard;
  std::vector<Particle> complete;

  bool build_index();
  const std::vector<std::vector<size_t>> &get_hard_index();
  const std::vector<std::vector<size_t>> &get_complete_index();
  bool build_information();

  size_t prolong = (size_t)0;

private:
  std::vector<std::vector<size_t>> hard_index;
  std::vector<std::vector<size_t>> complete_index;
  size_t no_width = (size_t)0;
  size_t name_width = (size_t)0;
  size_t e_width = (size_t)0;
  size_t phase_width = (size_t)0;
  bool hard_index_built = false;
  bool complete_index_built = false;
  bool information_built = false;

  static bool build_index(
      std::vector<std::vector<size_t>> &, std::vector<Particle> &);
  bool build_information(
      const std::vector<std::vector<size_t>> &, std::vector<Particle> &);
  bool preprocess();
  static bool preprocess(std::vector<Particle> &particles);
  static size_t get_phase(
      size_t no, std::vector<Particle> &particles);
  std::ostream &print(std::ostream &,
      const std::vector<std::vector<size_t>> &,
      const std::vector<Particle> &) const;
  std::ostream &print_all(std::ostream &,
      const std::vector<std::vector<size_t>> &,
      std::vector<Particle>) const;
};
