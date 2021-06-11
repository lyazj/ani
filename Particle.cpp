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

#include "Particle.h"

#include <iomanip>
#include <type_traits>

using namespace std;

istream &operator>>(istream &is, Particle_Base &pb)
{
  is >> pb.no >> pb.id >> pb.name >> pb.status;
  for(size_t &i : pb.mothers)
    is >> i;
  for(size_t &i : pb.daughters)
    is >> i;
  for(short &i : pb.colours)
    is >> i;
  is >> pb.p >> pb.e >> pb.m;
  return is;
}

istream &operator>>(istream &is, Particle &p)
{
  Particle_Base &pb(p);
  is >> pb;
  if(!is)
    return is;

  p.mothers.clear();
  for(size_t i : pb.mothers)
    if(i)
      p.mothers.insert(i);
  if(p.mothers.empty() && pb.no)
    p.mothers.insert(0);

  p.daughters.clear();
  for(size_t i : pb.daughters)
    if(i)
      p.daughters.insert(i);

  p.r = {0.0};
  p.v = pb.getv();
  p.phase = PHASE_UNDEF;
  p.death = PHASE_UNDEF;
  return is;
}

Particle_Printer &Particle_Printer::operator<<(const Particle &p)
{
  os << right;
  os << setw(no_width) << p.no << "  ";
  os << left;
  os << setw(name_width) << p.name << "  ";
  os << right;
  os << setw(e_width) << p.r[0] << "  ";
  os << setw(e_width) << p.r[1] << "  ";
  os << setw(e_width) << p.r[2] << "  ";
  os << setw(e_width) << p.v[0] << "  ";
  os << setw(e_width) << p.v[1] << "  ";
  os << setw(e_width) << p.v[2] << "  ";
  os << setw(e_width) << p.e << "  ";
  os << setw(e_width) << p.m << "  ";
  os << setw(phase_width)
     << typename make_signed<size_t>::type(p.death);
  return *this;
}

void Particle_Printer::print_header()
{
  os << right;
  os << setw(no_width) << "no" << "  ";
  os << left;
  os << setw(name_width) << "name" << "  ";
  os << right;
  os << setw(e_width) << "x" << "  ";
  os << setw(e_width) << "y" << "  ";
  os << setw(e_width) << "z" << "  ";
  os << setw(e_width) << "v_x" << "  ";
  os << setw(e_width) << "v_y" << "  ";
  os << setw(e_width) << "v_z" << "  ";
  os << setw(e_width) << "e" << "  ";
  os << setw(e_width) << "m" << "  ";
  os << setw(phase_width) << "death";
}
