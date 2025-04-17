#include "em.h"

/**************************************************************/
/* em class functions *****************************************/
/**************************************************************/


neutron::neutron()
{
  m_energy(0),
  m_x(0),
  m_y(0),
  m_theta(0),
  m_isDead(false),
  m_DeathType(-100)
}
neutron::neutron(double a_en,double a_x,double a_y, double)
{
  m_energy(0),
  m_x(0),
  m_y(0),
  m_theta(0),
  m_isDead(false),
  m_DeathType(-100)
}
double neutron::getVelocity()
{
  return 7.0;
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
  xp=a_pos.first;
  yp=a_pos.second;
  setPos(xp,yp);
}
double neutron::getE()
{
  return m_energy;
}
double neutron::getAngle()
{
  return m_theta;
}
void neutron::setAngle(double a_angle)
{
  m_theta=a_angle;
}
double neutron::getSteppedPos(double a_dist)
{
  double xn=m_x+std::cos(m_theta)*a_dist;
  double yn=m_y+std::sin(m_theta)*a_dist;
  std::pair<double,double>np={xn,yn};
  return np
}
void neutron::stepNewPos(double a_dist)
{
  m_x=m_x+std::cos(m_theta)*a_dist;
  m_y=m_y+std::sin(m_theta)*a_dist;
}
void neutron::kill(a_type)
{
  m_isDead=true;
  m_DeathType=a_type;
}