#include "System.h"
#include "Utility.h"

#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <iomanip>

#include <stdint.h>

using namespace std;

constexpr double duration = 1.0;

istream &operator>>(istream &is, System &s)
{
  string buf;
  while(getline(is, buf) &&
    buf.find("PYTHIA Process Initialization") == buf.npos);
  while(getline(is, buf) && (buf = strip(buf)).empty());
  s.description = buf;

  while(getline(is, buf) &&
    buf.find("PYTHIA Event Listing  (hard process)") == buf.npos);
  while(getline(is, buf) && (buf = strip(buf)).empty());
  if(!check_headline(buf))
  {
    cerr << "ERROR: Event listing headline check failed." << endl;
    is.setstate(is.failbit);
    return is;
  }
  istream_iterator<Particle> is_beg(is), is_end;
  s.hard.assign(is_beg, is_end);
  if(s.hard.empty())
  {
    cerr << "ERROR: Empty hard process list." << endl;
    is.setstate(is.failbit);
    return is;
  }

  is.clear();
  buf.clear();
  while(getline(is, buf) &&
    buf.find("PYTHIA Event Listing  (complete event)") == buf.npos);
  while(getline(is, buf) && (buf = strip(buf)).empty());
  if(!check_headline(buf))
  {
    cerr << "ERROR: Event listing headline check failed." << endl;
    is.setstate(is.failbit);
    return is;
  }
  s.complete.assign(is_beg = is, is_end);
  if(s.complete.empty())
  {
    cerr << "ERROR: Empty complete process list." << endl;
    is.setstate(is.failbit);
    return is;
  }

  if(!s.preprocess())
  {
    is.setstate(is.failbit);
    return is;
  }

  is.clear();
  return is;
}

bool System::preprocess()
{
  return preprocess(hard) && preprocess(complete);
}

bool System::preprocess(Subsystem &particles)
{
  for(size_t i = 0; i < particles.size(); ++i)
  {
    if(particles[i].no != i)
    {
      cerr << "ERROR: Wrong order of event listing." << endl;
      return false;
    }
    for(size_t d : particles[i].daughters)
      particles[d].mothers.insert(i);
    for(size_t m : particles[i].mothers)
      particles[m].daughters.insert(i);
  }
  return true;
}

bool System::build_index()
{
  if(!hard_index_built)
    hard_index_built = build_index(hard_index, hard);
  if(!complete_index_built)
    complete_index_built = build_index(complete_index, complete);
  return hard_index_built && complete_index_built;
}

bool System::build_index(Index &index, Subsystem &particles)
{
  try {
    for(size_t i = 0; i < particles.size(); ++i)
    {
      size_t phase(get_phase(i, particles));
      if(index.size() < phase + 1)
        index.resize(phase + 1);
      index[phase].push_back(i);
    }
  } catch(const runtime_error &err) {
    for(size_t i = 0; i < particles.size(); ++i)
      particles[i].phase = PHASE_UNDEF;
    index.clear();
    cerr << err.what() << endl;
    return false;
  }
  return true;
}

size_t System::get_phase(size_t no, Subsystem &particles)
{
  if(particles[no].phase == PHASE_UNDEF)
  {
    static size_t depth(0);
    if(depth == particles.size())
      throw runtime_error("ERROR: Maximum recursive depth exceeded.");
    ++depth;
    particles[no].phase = PHASE_MIN;
    for(size_t m : particles[no].mothers)
    {
      particles[no].phase = max(
          particles[no].phase, get_phase(m, particles) + 1);
    }
    --depth;
  }
  return particles[no].phase;
}

const vector<vector<size_t>> &System::get_hard_index()
{
  if(!hard_index_built &&
      !(hard_index_built = build_index(hard_index, hard)))
    throw runtime_error(
        "ERROR: Failed building index of hard process.");
  return hard_index;
}

const vector<vector<size_t>> &System::get_complete_index()
{
  if(!complete_index_built &&
      !(complete_index_built = build_index(complete_index, complete)))
    throw runtime_error(
        "ERROR: Failed building index of complete process.");
  return complete_index;
}

bool System::build_information()
{
  if(!information_built && !(information_built =
        build_information(get_hard_index(), hard) &&
        build_information(get_complete_index(), complete)))
    return false;
  e_width = to_string((int64_t)complete[0].e).size() + 5;
  phase_width = max(to_string(complete_index.size()).size() + 1, (size_t)5);
  no_width = to_string(complete.size()).size() + phase_width + 1;
  return true;
}

bool System::build_information(const Index &index, Subsystem &particles)
{
  for(size_t p = 0; p < index.size(); ++p)
  {
    for(size_t i : index[p])
    {
      double e(0.0);
      particles[i].r = {0.0};
      for(size_t m : particles[i].mothers)
      {
        e += particles[m].e;
        particles[i].r += particles[m].e * (particles[m].r +
            particles[m].v * ((p - particles[m].phase) * duration));
        particles[m].death = min(particles[m].death, p);
      }
      if(e)
        particles[i].r /= e;
      name_width = max(name_width, particles[i].name.size());
    }
  }
  return true;
}

ostream &System::
print(ostream &os, const Index &index, const Subsystem &particles) const
{
  Particle_Printer printer(os, no_width, name_width, e_width, phase_width);
  for(size_t p = 0; p < index.size(); ++p)
  {
    os << "[" << p << "]" << endl;
    printer << hdrp << endp;
    for(size_t i : index[p])
      printer << particles[i] << endp;
    os << endl;
  }
  return os;
}

ostream &System::
print_all(ostream &os, const Index &index, Subsystem particles) const
{
  Particle_Printer printer(os, no_width, name_width, e_width, phase_width);
  for(size_t p = 0; p < index.size(); ++p)
  {
    os << "[" << p << "]" << endl;
    printer << hdrp << endp;
    for(size_t pp = 0; pp <= p; ++pp)
      for(size_t i : index[pp])
        if(particles[i].death > p)
        {
          printer << particles[i] << endp;
          particles[i].r += particles[i].v * duration;
        }
    os << endl;
  }

  if(prolong)
  {
    vector<size_t> remaining;
    for(size_t i = 0; i < particles.size(); ++i)
      if(particles[i].death == PHASE_UNDEF)
        remaining.push_back(i);
    for(size_t t = 0; t < prolong; ++t)
    {
      os << "[" << index.size() + t << "]" << endl;
      printer << hdrp << endp;
      for(size_t i : remaining)
      {
        printer << particles[i] << endp;
        particles[i].r += particles[i].v * duration;
      }
      os << endl;
    }
  }

  return os;
}

ostream &operator<<(ostream &os, System &_s)
{
  _s.build_information();
  System s(_s);
  os << fixed << setprecision(3);
  os << "********** hard process (new) **********\n" << endl;
  s.print(os, s.hard_index, s.hard);
  os << "********** end of listing **********\n" << endl;
  os << "********** hard process (all) **********\n" << endl;
  s.print_all(os, s.hard_index, s.hard);
  os << "********** end of listing **********\n" << endl;
  os << "********** complete process (new) **********\n" << endl;
  s.print(os, s.complete_index, s.complete);
  os << "********** end of listing **********\n" << endl;
  os << "********** complete process (all) **********\n" << endl;
  s.print_all(os, s.complete_index, s.complete);
  os << "********** end of listing **********\n" << endl;
  return os << defaultfloat;
}
