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
  if(argc!=5)
  {
    std::cout<<"invalid arg count"<<std::endl;
    std::cout<<"correct use should be ./main {num particles} {grid per cm^2} {geo file} {baseOutName}"<<std::endl;
    return 1;
  }
  double epk=0.0005;
  int burn = 50;
  int active = 105;
  // begin material intialization
  materialManager* mat = new materialManager();
  randomGen* randy = new randomGen();
  // randomGen* randy = new randomGen(15681);

  // begin geometry pu/fe only
  // std::pair<double,double> dims = mat->addShapeFromFile("../neutron-mc/geos/twoCubes.txt");
   std::pair<double,double> dims = mat->addShapeFromFile(argv[3]);
  // std::pair<double,double> dims = mat->addShapeFromFile("../neutron-mc/geos/incased.txt");
  // put fe and c
  // std::pair<double,double> dims = mat->addShapeFromFile("../neutron-mc/geos/checkers.txt");

  // add cx data
  mat->addMaterial("fe",{{"abs","../neutron-mc/crossSecs/ironAbs.csv"},{"tot","../neutron-mc/crossSecs/ironAbs.csv"}});
  // mat->addMaterial("pu",{{"fis","../neutron-mc/crossSecs/plutoniumFis.csv"},{"abs","../neutron-mc/crossSecs/plutoniumAbs.csv"},{"tot","../neutron-mc/crossSecs/plutoniumTot.csv"}});
  // mat->addMaterial("c",{{"abs","../neutron-mc/crossSecs/carbonAbs.csv"},{"tot","../neutron-mc/crossSecs/carbonTot.csv"}});
  // mat->addNu("pu",2.8836);
  // mat->addDensity("pu",19.86,239);
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
  // int kone=999999999;
  // std::vector<double> keffCon;
  // std::vector<double> EntCon;
  for(int i; i<numpart ; i++)
  {
    neutron nuet = neutron(1000,0,
                          ydim*randy->getNormRand(),
                          0);
    // std::cout << "Made neutron " <<i<<" successfully" << std::endl;
    // std::cout << nuet.getPos().first  <<" : "<< nuet.getPos().second<< std::endl;
    // fs->setFissionSite(nuet,1);
    // std::cout << "Set neutron fission " <<i<<" successfully" << std::endl;    
    curState.addNeutron(nuet);
  }
  // int fisNum;
  // int oldFis;
  // double entNum;
  // bool kcon=false;
  // int ksize;
  reducedState tallied = reducedState();
  stepper stepping = stepper();


  // transport neutrons until convergence/loop max
  for (int j=0 ; j<burn+active ; j++)
  {
    fs->clear();
    stepping.setState(curState);
    std::vector<neutron> stepped = stepping.step(mat, randy,fs);
    // fisNum=fs->getTotalFissions();
    state curState = state(stepped);
    if(j>burn)
    {
      tallies tally = tallies(curState, mat,100,numpart);
      tallied.addReduced(tally);
      tally.~tallies();
      // std::cout<<double(fisNum)/oldFis<<std::endl;
      // tallied.addMC(double(fisNum)/numpart,entNum);
    }
    for(int i; i<numpart ; i++)
  {
    neutron nuet = neutron(1000,0,
                          ydim*randy->getNormRand(),
                          0);
    // std::cout << "Made neutron " <<i<<" successfully" << std::endl;
    // std::cout << nuet.getPos().first  <<" : "<< nuet.getPos().second<< std::endl;
    // fs->setFissionSite(nuet,1);
    // std::cout << "Set neutron fission " <<i<<" successfully" << std::endl;    
    curState.addNeutron(nuet);
  }
    // oldFis=fisNum;
    
    
    // fs->~MCstats();
    // std::cout << "Deleted fsStart successfully"<<std::endl;
  }
  // for(int i ; i<tallied.getK().size() ; i++)
  //   {
  //     if (i%5==0&&i>kone)
  //     {
  //       std::cout<<tallied.getK()[i]<<std::endl;
  //     }
  //   }
  std::vector<double> trans;
    for(tallies tal : tallied.getTallies())
    {
      trans.push_back(tal.getEscape());
    }
    std::pair<double, double> transStat=fs->getStats(trans);
  // end loop and output values
    std::string baseName=argv[4];
    std::ofstream outFile(baseName+"Main.txt");
    outFile<<"escape  "<<"std escape"<<std::endl;
    outFile<<transStat.first<<"   "<<transStat.second<<std::endl;
    double cx=0.6022*(mat->getCX("fe","abs",1000))*(mat->getDens()["fe"]);
    outFile<<"analytical:   "<<std::exp(-1.0*cx*dims.first);
    outFile.close();
    std::vector<double> entHist = tallied.getEnt();
    std::ofstream plotFile(baseName+"Data.dat");
    // plotFile<<std::setw(80);
    // plotFile<<std::setfill(' ');
    plotFile<<"cycle "<<"trans frac"<<"\n";
    for(int i=0 ; i<trans.size()-3;i++)
    {
      plotFile<<" "<<i<<" "<<trans[i]<<"\n";
    }
    plotFile.close();

  return 0;
}
