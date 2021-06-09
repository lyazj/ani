#include "Interface.h"
#include "System.h"

#include <fstream>

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
  istream *pis;
  if(infile == "-")
    pis = &cin;
  else
  {
    ifs.open(infile);
    pis = &ifs;
  }

  ofstream ofs;
  ostream *pos;
  if(outfile == "-")
    pos = &cout;
  else
  {
    ofs.open(outfile);
    pos = &ofs;
  }

  return simulate(*pis, *pos, prolong);
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
