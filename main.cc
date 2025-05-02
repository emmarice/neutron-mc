// main program?
#include "include/jh.h"
#include "src/jh.cpp"
#include "include/em.h"
#include "src/em.cc"
#include <iostream>

/*
  The arguments for main will take in order:
  The number of particles to be run
  The length in x in cm
  The length in y in cm
  the number of grids per cm
*/
int main(int argc, char** argv)
{
  int numpart = atoi(argv[1]);
  float xdim = atof(argv[2]);
  float ydim = atof(argv[3]);
  int grids = atoi(argv[4]);
  MCstats fs = MCstats(xdim,ydim,grids);
  neutron neut = neutron(1,3.5,2.6,40);
  fs.setFissionSite(neut);
  fs.printFissionSites();
  neutron neut2 = neutron(1,4.3,1.2,40);
  fs.setFissionSite(neut2,99);
  fs.printFissionSites();




  return 0;
}
