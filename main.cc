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
  int active = 105;
  // begin material intialization
  materialManager* mat = new materialManager();
  // randomGen* randy = new randomGen();
  randomGen* randy = new randomGen(15681);

  // begin geometry pu/fe only
  // std::pair<double,double> dims = mat->addShapeFromFile("../neutron-mc/geos/twoCubes.txt");
   std::pair<double,double> dims = mat->addShapeFromFile("../neutron-mc/geos/jezebelsquircle.txt");
  // std::pair<double,double> dims = mat->addShapeFromFile("../neutron-mc/geos/incased.txt");
  // put fe and c
  // std::pair<double,double> dims = mat->addShapeFromFile("../neutron-mc/geos/checkers.txt");

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
  int fisNum;
  int oldFis;
  double entNum;
  bool kcon=false;
  int ksize;
  std::vector<double> ksHist;
  reducedState tallied = reducedState();
  stepper stepping = stepper();


  // transport neutrons until convergence/loop max
  for (int j=0 ; j<burn+active ; j++)
  {
    fs->clear();
    stepping.setState(curState);
    std::vector<neutron> stepped = stepping.step(mat, randy,fs);
    fisNum=fs->getTotalFissions();
    state curState = state(stepped);
    if(j>burn)
    {
      tallies tally = tallies(curState, mat,fisNum,numpart);
      tallied.addReduced(tally);
      tally.~tallies();
      // std::cout<<double(fisNum)/oldFis<<std::endl;
      tallied.addMC(double(fisNum)/oldFis,entNum);
      if(j%5==0)
      {
        ksHist=tallied.getK();
        ksize=ksHist.size();
        if(std::pow((ksHist[ksize-1]-ksHist[ksize-2]),2)/std::pow(ksHist[ksize-2],2)<epk||kcon)
        {
          if(!kcon)
          {
            kcon=true;
            kone=j-burn;
            std::cout<<"k has converged at cycle burn+"<<j-burn<<std::endl;
          }
          keffCon.push_back(double(fisNum)/oldFis);
          EntCon.push_back(entNum);
        }
      }
    }
    curState=fs->nextState(numpart,randy);
    entNum=fs->getEntropy();
    oldFis=fisNum;
    
    
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
    std::vector<double> pathEst;
    std::vector<double> absEst;
    std::vector<double> colEst;
    std::vector<double> pathEstCon;
    std::vector<double> absEstCon;
    std::vector<double> colEstCon;
    double oldvalP;
    double oldvalA;
    double oldvalC;
    bool skip=true;
    int i=0;
    for(tallies tal : tallied.getTallies())
    {
      if(skip)
      {
        oldvalP=tal.getPathEst();
        oldvalA=tal.getAbsEst();
        oldvalC=tal.getColEst();
        skip=false;
        std::cout<<oldvalP<<std::endl;
        std::cout<<oldvalA<<std::endl;
        std::cout<<oldvalC<<std::endl;
      }
      else
      {
        if(oldvalP!=0&&oldvalA!=0&&oldvalC!=0)
        {
          pathEst.push_back(tal.getPathEst()/oldvalP);
          absEst.push_back(tal.getAbsEst()/oldvalA);
          colEst.push_back(tal.getColEst()/oldvalC);
          if(i>kone)
          {
            pathEstCon.push_back(tal.getPathEst()/oldvalP);
            absEstCon.push_back(tal.getAbsEst()/oldvalA);
            colEstCon.push_back(tal.getColEst()/oldvalC);
          }
        }
        else
        {
          std::cout<<"things done did broke"<<std::endl;
          skip=true;
        }
        oldvalP=tal.getPathEst();
        oldvalA=tal.getAbsEst();
        oldvalC=tal.getColEst();
      }
      i++;
    }
    std::pair<double, double> kstat=fs->getStats(keffCon);
    std::pair<double, double> entstat=fs->getStats(EntCon);
    std::pair<double, double> pathstat=fs->getStats(pathEstCon);
    std::pair<double, double> absstat=fs->getStats(absEstCon);
    std::pair<double, double> colstat=fs->getStats(colEstCon);
  // end loop and output values

    std::ofstream outFile("chekerMain.txt");
    outFile<<std::setw(80);
    outFile<<std::setfill(' ');
    outFile<<"Main out Put\n";
    outFile<<"k mean " <<"k std "<<"Entropy mean "<<"Entropy std "<<"k (path) mean "
    <<"k (path) std "<<"k (absorb) mean "<<"k (absorb) std "<<"k (colide) mean "<<"k (colide) std\n";
    outFile<<kstat.first<<" "<<kstat.second<<" "<<entstat.first<<" "<<entstat.second<<" "<<pathstat.first<<" "
    <<pathstat.second<<" "<<absstat.first<<" "<<absstat.second<<" "<<colstat.first<<" "<<colstat.second<<" ";
    outFile.close();
    std::vector<double> entHist = tallied.getEnt();
    std::ofstream plotFile("checkerData.dat");
    plotFile<<std::setw(80);
    plotFile<<std::setfill(' ');
    plotFile<<"cycle "<<"k "<<"Ent "<<"path "<<"abs "<<"col "<<"\n";
    for(int i=0 ; i<pathEst.size();i++)
    {
      plotFile<<" "<<i<<" "<<ksHist[i]<<" "<<entHist[i]<<" "
      <<pathEst[i]<<" "<<absEst[i]<<" "<<colEst[i]<<"\n";
    }
    plotFile.close();
    fs->saveFissionSites("checkerFis.dat");

  return 0;
}
