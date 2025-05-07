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
#include <iomanip>
#include <fstream>
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
    std::cout<<"correct use should be ./main {num particles} {grid per cm^2}"<<std::endl;
    return 1;
  }
  double epk=0.0005;
  int burn = 50;
  int active = 100;
  // begin material intialization
  materialManager* mat = new materialManager();
  // randomGen* randy = new randomGen();
  randomGen* randy = new randomGen(86753);

  // begin geometry pu/fe only
  std::pair<double,double> dims = mat->addShapeFromFile("../neutron-mc/geos/twoCubes.txt");
  // std::pair<double,double> dims = mat->addShapeFromFile("./geos/incased.txt");
  // put fe and c
  // std::pair<double,double> dims = mat->addShapeFromFile("./geos/checkers.txt");

  // add cx data
  mat->addMaterial("fe",{{"abs","../neutron-mc/crossSecs/ironAbs.csv"},{"tot","../neutron-mc/crossSecs/ironTot.csv"}});
  mat->addMaterial("pu",{{"fis","../neutron-mc/crossSecs/plutoniumFis.csv"},{"abs","../neutron-mc/crossSecs/plutoniumAbs.csv"},{"tot","../neutron-mc/crossSecs/plutoniumTot.csv"}});
  // mat->addMaterial("c",{{"abs","../neutron-mc/crossSecs/carbonAbs.csv"},{"tot","../neutron-mc/crossSecs/carbonTot.csv"}});
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
  MCstats * fs = new MCstats(xdim,ydim,grids);
  // begin state intialization
  state curState;
  int kone=999999999;
  std::vector<double> keffCon;
  std::vector<double> EntCon;
  for(int i; i<numpart ; i++)
  {
    neutron nuet = neutron(fs->sampleEnergy(randy),xdim*randy->getNormRand(),
                          ydim*randy->getNormRand(),
                          4*std::acos(0.0)*randy->getNormRand());
    fs->setFissionSite(nuet,1);
    curState.addNeutron(nuet);
  }