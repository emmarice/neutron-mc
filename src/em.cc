#include "../include/em.h"

/**************************************************************/
/* em class functions *****************************************/
/**************************************************************/


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
  m_step=a_dist;
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
bool neutron::getCol()
{
  return m_col;
}
bool neutron::getIsDead()
{
  return m_isDead;
}
double neutron::getStep()
{
  return m_step;
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

void state::step(MCstats* fish, materialManager * mat, randomGen * rgen) // steps forward in time, changing the vector of neutrons for the state
{
  m_particles_count = m_particles.size(); //length of m_neutrons
  // if m_neutrons is empty print a warning?
  if (m_particles_count < 1)
  {
    cout << "***\nYour state has zero neutrons in it :(\n***" << endl;
  }
  // else do the things below
    // for each neutron in m_neutrons:
    for (int i = 0; i < m_particles_count; ++i)
    {
      //while alive:
      while (!m_particles[i].getIsDead())
      {
        // sample distance to interaction
        string mtype = mat->matFinder(m_particles[i].getPos());
        double distance = mat->getDist(mtype, m_particles[i].getPos());
        std::pair<double,double> newpos = m_particles[i].getSteppedPos(distance);
        string newmtype = mat->matFinder(newpos);
        // if boundary is being crossed:
        if (mtype != newmtype)
        {
          // move to boundary
          m_particles[i].setPos(mat->findBound(newpos, m_particles[i]));
          // update material
          mtype = mat->matFinder(m_particles[i].getPos());
        }
        else
        {
          m_particles[i].stepNewPos(distance);
        }

        // if escaped:
        if (mtype == "void")
        {
          // kill neutron
          m_particles[i].killN(0);
          // tally?
        }  
        else // some type of collision will happen
        {
          // sample reaction type
          string rx = mat->getReactionType(rgen->getNormRand(), mtype, m_particles[i].getE())
          // if absorbed:
          if (rx == "abs")
          {
            // kill neutron
            m_particles[i].killN(1);
            // tally?
          }
          // if fissioned:
          else if (rx == "fis")
          {
            // kill neutron
            m_particles[i].killN(2);
            // sample number of neutrons produced & save for next gen
            fish->setFissionSite(m_particles[i], getFisInfo(mtype, rgen->getNormRand(),rgen->getNormRand()));
          }
          // if scattered:
          else if (rx == "scat")
          {
            // sample new energy and angle
            m_particles[i].setE(mat->getScatEn(m_particles[i].getE(), rgen->getNormRand(), mtype));
            m_particles[i].setAngle(rgen->getNormRand()*2*std::numbers::pi);
          }
          else
          {
            cout << "the reaction type isn't matching abs, scatter, or fission. weird!" << endl;
          }
        } // end collisions   
      } // end while alive
    } // end for each neutron
    // end of one cycle == end of step
}// end of state::step
