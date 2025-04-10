#ifndef __KE_H
#define __KE_H

#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// manages materials and getting crossection
class materialManager
{
  public:
    std::map<std::string,std::vector<double,double>> m_crossX;
  public:
    void addMaterial(std::string,std::vector<double,double>);
    double getCX(double a_En);
}
// reduces state to tallies and saves them
class tallies
{
  public:
    double m_AvEnergy;
    double m_
}

#endif