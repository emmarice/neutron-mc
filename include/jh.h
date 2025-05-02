#ifndef __JH_H
#define __JH_H

#include <vector>
#include "em.h"
#include "ke.h"

class MCstats
{
public:
  // Pointer to a series of pointers which is the location of our fission sites
  float ** m_fission_sites;
  fission_sites();
  MCstats(float a_x, float a_y, int grains);
  // Sets the size of the array of fission sites, points to [0,0]
  void setFissionSite(neutron a_neutron);
  void setFissionSite(neutron a_neutron, float a_n);
  float ** allocate(int a_row, int a_col);
  void deallocate();
  float * getRow(int row);
  bool fisCon(std::vector<neutron>);
  void setDims(float a_x, float a_y, int grains);
  void printFissionSites();
  ~MCstats();


private:
  int m_row;
  int m_col;
  int m_grains;

};


#endif