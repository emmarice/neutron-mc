#ifndef __JH_H
#define __JH_H

#include <vector>
#include <utility>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
// #include "em.h"
#include "baseClasses.h"
#include "ke.h"

class MCstats
{
public:
  // Pointer to a series of pointers which is the location of our fission sites
  double ** m_fission_sites;
  // fission_sites();
  MCstats(double a_x, double a_y, int grains);
  // Sets the size of the array of fission sites, points to [0,0]
  void setFissionSite(neutron a_neutron);
  void setFissionSite(neutron a_neutron, int a_n);
  double ** allocate(int a_row, int a_col);
  void deallocate();
  double * getRow(int row);
  bool fisCon(std::vector<neutron>);
  void setDims(double a_x, double a_y, int grains);
  void printFissionSites();
  void normalizeSites();
  int getTotalFissions();
  state nextState();
  ~MCstats();
  double sampleEnergy(randomGen * a_rand);
  state nextState(int a_numParticles,randomGen * rgen);
  double getEntropy();
  void clear();
  std::pair<double, double> getStats(std::vector<double> a_vec);
  void saveFissionSites(std::string a_of);

private:
  int m_row;
  int m_col;
  int m_grains;
  int m_totalParts=0;
  std::vector<double> m_normSites;
  std::vector<std::pair <int,int>> m_locations;

};


#endif