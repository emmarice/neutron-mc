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
#include "baseClasses.h"
// manages materials and getting crossection
class randomGen
{
  private:
    int m_a;
    int m_b;
    int m_m;
    long m_xi;
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
    // cross sections accepted: "fis" (fission), "abs" (absorb), "tot" (total for scater)
    std::map<std::string,std::map<std::string,std::vector<std::pair<double,double>>>> m_crossX;
    std::map<std::string,std::vector<std::string>> m_matTypes;
    std::map<std::string,double> m_matDens;
    std::vector<std::string> m_mats;
    std::vector<std::string> m_regions;
    std::map<std::string,double> m_nuBar;
    std::map<std::string,int> m_As;
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
    std::pair<double,double> findBound(neutron a_nautron, double a_d);
    void addNu(std::string a_matName,double a_nu);
    std::map<std::string,double> getNus();
    // add number density or density calculated if A>0
    // a rho either in mol/cm^3 or g/cm^3
    void addDensity(std::string a_matName,double a_rho, double a_A=-100);
    std::string matFinder(double a_x, double a_y);
    std::string matFinder(std::pair<double,double> a_pos);
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
    double getScatEn(double a_En, double a_eta,std::string a_matName);
    int getFisInfo(std::string a_matName,
                                              double a_eta);
    // adds a rectangle to the model given corner points
    // give the name of the material, bottom corner and top corner
    void addShape(std::string a_reg, std::string a_mat,double a_xLow, double a_yLow,
                   double a_xHigh, double a_yHigh);
    std::pair<double,double> addShapeFromFile(std::string a_fileName);
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
    tallies();
    ~tallies();
    tallies(state a_state, materialManager* a_mats, int a_fisnum,int a_fisNor); 
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
  private:
    std::vector<tallies> m_reducedStates;
    std::vector<double> m_ks;
    std::vector<double> m_entropy;
  public:
    reducedState();
    void addReduced(tallies a_tally);
    void addMC(double a_k, double a_ent);
    std::vector<tallies> getTallies();
    std::vector<double> getK();
    std::vector<double> getEnt();
};

#endif
