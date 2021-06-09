#pragma once

#ifdef __cplusplus

#include <string>

int simulate(std::istream &is, std::ostream &os, int prolong = 0);
int simulate(const std::string &infile,
    const std::string &outfile, int prolong = 0);

extern "C" {

#endif

int simulate(const char *infile, const char *outfile);
int simulate_prolong(const char *infile, const char *outfile, int prolong);

#ifdef __cplusplus

}

#endif
