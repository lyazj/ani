#include "Interface.h"
#include "System.h"

#include <fstream>
#include <memory>

using namespace std;

int simulate(istream &is, ostream &os, int prolong)
{
  System system;
  system.prolong = prolong > 0 ? prolong : 0;
  return !(is >> system) || !(os << system);
}

int simulate(const string &infile, const string &outfile, int prolong)
{
  ifstream ifs;
  ofstream ofs;
  return simulate(infile == "-" ? cin : (ifs.open(infile), ifs),
      outfile == "-" ? cout : (ofs.open(outfile), ofs), prolong);
}

extern "C" {

int simulate(const char *infile, const char *outfile)
{
  return simulate(infile, outfile, 0);
}

int simulate_prolong(const char *infile, const char *outfile, int prolong)
{
  return simulate(infile, outfile, prolong);
}

}
