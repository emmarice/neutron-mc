// main program?
#include "include/jh.h"
#include "src/jh.cpp"
#include "include/ke.h"
#include "src/ke.cpp"
#include "include/em.h"
#include "src/em.cpp"

/*
  The arguments for main will take in order:
  The number of particles to be run
  The length in x in cm
  The length in y in cm
  the number of grids per cm
*/
int main(int argc, char* argv[])
{
  int numpart = argv[0];
  float xdim = argv[1];
  float ydim = argv[2];
  int grids = argv[3];
  fission_sites = new MCstats(xdim,ydim,grids);




  return 0;
}
