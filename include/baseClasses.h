#ifndef __BASECLASSES_H
#define __BASECLASSES_H

#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <iostream>
using std::pair;
#include <utility>
class neutron
{
private:
  double m_energy; // energy in keV
  double m_x; //x position as measured from -- in [units]
  double m_y; //y position as measured from -- in [units]
  double m_theta; //angle in radians
  bool m_isDead; // 0 alive, 1 dead
  int m_DeathType; //0 escape, 1 capture, 2 fission
  double m_weight;
  bool m_col; // did it colide
  std::string m_mat; //material
  std::vector<std::pair<std::string,double>> m_pathEst;
public:
  neutron();
  neutron(double a_en,double a_x,double a_y, double a_angle);
  ~neutron();
  /* void func()
      -brief: 
      -param: None
      -return: No return -- void
    */
double getE();
void setE(double a_E);
std::pair<double,double> getPos();
void setPos(double a_x,double a_y);
void setPos(std::pair<double,double> a_pos);
double getAngle();
void setAngle(double a_angle);
std::pair<double,double> getSteppedPos(double a_dist);
void stepNewPos(double a_dist);
void addStep(std::string a_mat, double a_dist);
void killN(int a_type);
int getDeath();
void setMat(std::string a_mat);
void setCol(bool a_col);
std::string getMat();
std::vector<std::pair<std::string,double>> getPathEst();
bool getCol();
bool getIsDead();

  
};
class state
{
public:
  std::vector<neutron> m_particles;
public:
  state();
  ~state();
  std::vector<neutron> getParticles();
  int getNumParticles();
  void addNeutron(neutron a_neut);
};
#endif