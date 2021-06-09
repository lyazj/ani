#include "Interface.h"

#include <iostream>

#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
  cin.sync_with_stdio(0);
  if(argc > 4)
  {
    cerr << "ERROR: Wrong number of arguments." << endl;
    return 1;
  }
  const char *infile(argc > 1 ? (const char *)argv[1] : "-");
  const char *outfile(argc > 2 ? (const char *)argv[2] : "-");
  int prolong(argc > 3 ? atoi(argv[3]) : 0);
  return simulate(infile, outfile, prolong);
}
