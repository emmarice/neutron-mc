#include "../include/baseClasses.h"
neutron::neutron()
{
  m_energy=0;
  m_x=0;
  m_y=0;
  m_theta=0;
  m_isDead=false;
  m_DeathType=-100;
}
neutron::neutron(double a_en,double a_x,double a_y, double a_angle)
{
  m_energy=a_en;
  m_x=a_x;
  m_y=a_y;
  m_theta=a_angle;
  m_isDead=false;
  m_DeathType=-100;
}

double neutron::getE()
{
  return m_energy;
}
void neutron::setE(double a_E)
{
  m_energy=a_E;
}
std::pair<double,double> neutron::getPos()
{
  std::pair<double,double> pos={m_x,m_y};
  return pos;
}
void neutron::setPos(double a_x,double a_y)
{
  m_x=a_x;
  m_y=a_y;
}
void neutron::setPos(std::pair<double,double> a_pos)
{
  double xp=a_pos.first;
  double yp=a_pos.second;
  setPos(xp,yp);
}
double neutron::getAngle()
{
  return m_theta;
}
void neutron::setAngle(double a_angle)
{
  m_theta=a_angle;
}
std::pair<double,double> neutron::getSteppedPos(double a_dist)
{
  double xn=m_x+std::cos(m_theta)*a_dist;
  double yn=m_y+std::sin(m_theta)*a_dist;
  std::pair<double,double>np={xn,yn};
  return np;
}
void neutron::stepNewPos(double a_dist)
{
  m_x=m_x+std::cos(m_theta)*a_dist;
  m_y=m_y+std::sin(m_theta)*a_dist;
  pair<std::string,double> outp={m_mat,a_dist};
  m_pathEst.push_back(outp);
}
void neutron::addStep(std::string a_mat, double a_dist)
{
  pair<std::string,double> outp={a_mat,a_dist};
  m_pathEst.push_back(outp);
}
void neutron::killN(int a_type)
{
  m_isDead=true;
  m_DeathType=a_type;
}
int neutron::getDeath()
{
  return m_DeathType;
}
void neutron::setMat(std::string a_mat)
{
  m_mat=a_mat;
}
void neutron::setCol(bool a_col)
{
  m_col=a_col;
}
std::string neutron::getMat()
{
  return m_mat;
}
std::vector<std::pair<std::string,double>> neutron::getPathEst()
{
  return m_pathEst;
}
bool neutron::getCol()
{
  return m_col;
}
bool neutron::getIsDead()
{
  return m_isDead;
}
state::state()
{
  // nothing to do here
}
std::vector<neutron> state::getParticles()
{
  return m_particles;
}
int state::getNumParticles()
{
  return m_particles.size();
}
void state::addNeutron(neutron a_neut)
{
  m_particles.push_back(a_neut);
}

neutron::~neutron()
{
  std::cout<< "Neutron deleted!"<< std::endl;
} //end of neutron::~neutron