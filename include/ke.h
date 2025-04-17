#ifndef __KE_H
#define __KE_H

#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include em.h
// manages materials and getting crossection
class materialManager
{
  public:
    // maps material to type of cross section to values (energy,cx)
    std::map<std::string,std::map<std::string,std::vector<std::pair<double,double>>>> m_crossX;
    std::map<std::string,vector<std::string>> m_matTypes;
    std::map<std::string,double> m_matDens;
    std::vector<std::string> m_mats;
    // map of material to pair of x bounds and y bounds
    std::map<std::string,std::pair<std::pair<double,double>,std::pair<double,double>>> m_geo
  public:
    materialManager();
    // adds given CX from JANIS file
    void addCX(std::string a_matName,std::string a_CXType,
                std::string a_fileName);
    //adds name of material with vector of reaction name and file name
    void addMaterial(std::string a_matName, double a_rho,
                    std::vector<std::pair<std::string,std::string>> a_files);
    std::string matFinder(double a_x, double a_y);
    // get CX at given energy in material
    double getCX(std::string a_matName,std::string a_type,double a_En);
    // get total cx
    double getCXTot(std::string a_matName,double a_En);
    // sample the distance to interaction
    double getDist(std::string a_matName,double a_En, double a_eta);
    std::string getReactionType(double a_eta,std::string a_matName,
                                double a_En);
    void addShape(std::string a_mat,double a_xLow, double a_yLow,
                   double a_xHigh, double a_yHigh);
}
// reduces state to tallies and saves them
class tallies
{
  private:
    double m_avEnergy;
    double m_avEscape;
    double m_avFis;
    double m_avCap;
    double m_avSurvive;
    int m_num;
  public:
    tallies(state a_state); 
    double getAvEn();
    double getEscape();
    double getFis();
    double getCap();
    double getAlive();
    int getNum();

}
class reducedState
{
  public:
    std::vector<tallies> m_reducedStates;
  public:
    reducedState();
    void addReduced(tallies a_tally);
    std::vector<tallies> getTallies();
}
#endif