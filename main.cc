// main program?
#include "include/baseClasses.h"
#include "src/baseClasses.cpp"
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
  the number of grids per cm
*/
int main(int argc, char** argv)
{
  if(argc!=3)
  {
    std::cout<<"invalid arg count"<<std::endl;
    std::cout<<"correct use should be ./main {num particles} {grid per cm}"<<std::endl;
    return 1;
  }
  // begin material intialization
  materialManager* mat = new materialManager();
  randomGen* randy = new randomGen();
  // randomGen* randy = new randomGen(seed);

  // begin geometry pu/fe only
  std::pair<double,double> dims = mat->addShapeFromFile("../neutron-mc/geos/twoCubes.txt");
  // std::pair<double,double> dims = mat->addShapeFromFile("./geos/incased.txt");
  // put fe and c
  // std::pair<double,double> dims = mat->addShapeFromFile("./geos/checkers.txt");

  // add cx data
  mat->addMaterial("fe",{{"abs","../neutron-mc/crossSecs/ironAbs.csv"},{"tot","../neutron-mc/crossSecs/ironTot.csv"}});
  mat->addMaterial("pu",{{"fis","../neutron-mc/crossSecs/plutoniumFis.csv"},{"abs","../neutron-mc/crossSecs/plutoniumAbs.csv"},{"tot","../neutron-mc/crossSecs/plutoniumTot.csv"}});
  mat->addMaterial("c",{{"abs","../neutron-mc/crossSecs/carbonAbs.csv"},{"tot","../neutron-mc/crossSecs/carbonTot.csv"}});
  mat->addNu("pu",2.8836);
  mat->addDensity("pu",19.86,239);
  mat->addDensity("fe",7.874,56);
  // mat->addDensity("c",2.266,12); //graphite

  // end material initalization

  // start MC stats
  int numpart = atoi(argv[1]);
  float xdim = dims.first;
  float ydim = dims.second;
  int grids = atoi(argv[2]);
  MCstats fs = MCstats(xdim,ydim,grids);
  neutron neut = neutron(1,3.5,2.6,40);
  fs.setFissionSite(neut);
  fs.printFissionSites();
  neutron neut2 = neutron(1,4.3,1.2,40);
  fs.setFissionSite(neut2,99);
  fs.printFissionSites();



  return 0;
}
