#ifndef __KE_H
#define __KE_H

#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <map>
using std::pair;
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <sstream>
#include "em.h"
// manages materials and getting crossection
class randomGen
{
  private:
    int m_a;
    int m_b;
    int m_m;
    int m_xi;
  public:
    randomGen(int a_x);
    randomGen();
    void setSeed(int a_x);
    void setPars(int a_a,int a_b,int a_m);
    int getRand();
    double getNormRand();
};
class materialManager
{
  private :
    // maps material to type of cross section to values (energy,cx)
    // cross sections accepted: "fis" (fission), "abs" (absorb), "sca" (scatter)
    std::map<std::string,std::map<std::string,std::vector<std::pair<double,double>>>> m_crossX;
    std::map<std::string,std::vector<std::string>> m_matTypes;
    std::map<std::string,double> m_matDens;
    std::vector<std::string> m_mats;
    std::vector<std::string> m_regions;
    std::map<std::string,double> m_nuBar;
    // map of region to material and pair of x bounds and y bounds
    std::map<std::string,std::pair<std::string,std::pair<std::pair<double,double>,std::pair<double,double>>>> m_geo;
  public:
    materialManager();
    // adds given CX from JANIS file
    void addCX(std::string a_matName,std::string a_CXType,
                std::string a_fileName);
    std::map<std::string,std::map<std::string,std::vector<std::pair<double,double>>>> getCXS();
    std::map<std::string,double> getDens();
    //adds name of material with vector of reaction name and file name
    void addMaterial(std::string a_matName,
                    std::vector<std::pair<std::string,std::string>> a_files);
    void addNu(std::string a_matName,double a_nu);
    std::map<std::string,double> getNus();
    // add number density or density calculated if A>0
    // a rho either in mol/cm^3 or g/cm^3
    void addDensity(std::string a_matName,double a_rho, double a_A=-100);
    std::string matFinder(double a_x, double a_y);
    // get CX at given energy in material
    double getCX(std::string a_matName,std::string a_type,double a_En);
    // get total cx
    double getCXTot(std::string a_matName,double a_En);
    // sample the distance to interaction
    double getDist(std::string a_matName,double a_En, double a_eta);
    // returns reaction type given a random number, the material neutron is in
    // and the energy of the neutron
    std::string getReactionType(double a_eta,std::string a_matName,
                                double a_En);
    std::pair<int,std::vector<double>> getFisInfo(std::string a_matName,
                                              double a_eta, randomGen* a_rand);
    // adds a rectangle to the model given corner points
    // give the name of the material, bottom corner and top corner
    void addShape(std::string a_reg, std::string a_mat,double a_xLow, double a_yLow,
                   double a_xHigh, double a_yHigh);
    void addShapeFromFile(std::string a_fileName);
};
// reduces state to tallies and saves them
class tallies
{
  private:
    double m_avEnergy;
    double m_avEscape;
    double m_avFis;
    double m_avCap;
    double m_avSurvive;
    double m_colEst;
    double m_absEst;
    double m_pathEst;
    int m_num;
  public:
    tallies(state a_state,materialManager* a_mats); 
    double getAvEn();
    double getEscape();
    double getFis();
    double getCap();
    double getAlive();
    double getColEst();
    double getAbsEst();
    double getPathEst();
    int getNum();

};
class reducedState
{
  public:
    std::vector<tallies> m_reducedStates;
  public:
    reducedState();
    void addReduced(tallies a_tally);
    std::vector<tallies> getTallies();
};

#endif