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
    // maps material to type of cross section to values (energy,cx)
    std::map<std::string,std::map<std::string,std::vector<std::pair<double,double>>>> m_crossX;
    std::map<std::string,vector<std::string>> m_matTypes;
    std::map<std::string,double> m_matDens;
  public:
    // adds given CX from JANIS file
    void addCX(std::string a_matName,std::string a_CXType,
                std::string a_fileName);
    //adds name of material with vector of reaction name and file name
    void addMaterial(std::string a_matName, double a_rho,
                    std::vector<std::pair<std::string,std::string>> a_files);
    std::string matFinder(double x, double y);
    // get CX at given energy in material
    double getCX(std::string a_matName,std::string a_type,double a_En);
    // get total cx
    double getCXTot(std::string a_matName,double a_En);
    // sample the distance to interaction
    double getDist(std::string a_matName,double a_En, double a_eta);
    std::string getReactionType(double a_eta,std::string a_matName,
                                double a_En);
}
// reduces state to tallies and saves them
class tallies
{
  public:
    double m_avEnergy;
    double m_avEscape;

}

#endif