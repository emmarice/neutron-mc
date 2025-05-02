// main program?
#include "include/jh.h"
#include "src/jh.cpp"
#include "include/em.h"
#include "src/em.cc"
#include "include/ke.h"
#include "src/ke.cpp"
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

  // begin material intialization
  materialManager* mat = new materialManager();
  randomGen* randy = new randomGen();
  // randomGen* randy = new randomGen(seed);

  // begin geometry pu/fe only
  mat->addShapeFromFile("./geos/twoCubes.txt");
  // mat->addShapeFromFile("./geos/incased.txt");
  // put fe and c
  // mat->addShapeFromFile("./geos/checkers.txt");

  // add cx data
  mat->addMaterial("fe",{{"abs","./crossSecs/ironAbs.csv"},{"tot","./crossSecs/ironTot.csv"}});
  mat->addMaterial("pu",{{"fis","./crossSecs/plutoniumFis.csv"},{"abs","./crossSecs/plutoniumAbs.csv"},{"tot","./crossSecs/plutoniumTot.csv"}});
  mat->addMaterial("c",{{"abs","./crossSecs/carbonAbs.csv"},{"tot","./crossSecs/carbonTot.csv"}});
  mat->addNu("pu",2.8836);
  mat->addDensity("pu",19.86,239);
  mat->addDensity("fe",7.874,56);
  // mat->addDensity("c",2.266,12); //graphite


  return 0;
}
