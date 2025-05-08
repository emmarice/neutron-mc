#include "../include/em.h"
/**************************************************************/
/* em class functions *****************************************/
/**************************************************************/
stepper::stepper()
{

}

stepper::stepper(state a_state)
{
  // std::cout<<"load state"<<std::endl;
  m_state=a_state;
  m_particles=m_state.getParticles();
  // std::cout<<"loaded"<<std::endl;
}

void stepper::setState(state a_state)
{
  // std::cout<< "setting"<<std::endl;
  m_state=a_state;
  m_particles=m_state.getParticles();
  // std::cout<< "set"<<std::endl;
}

std::vector<neutron> stepper::step( materialManager * mat, randomGen * rgen, MCstats* fish) // steps forward in time, changing the vector of neutrons for the state
{
  m_particles_count = m_state.getNumParticles(); //length of m_neutrons
  // if m_neutrons is empty print a warning?
  if (m_particles_count < 1)
  {
    std::cout << "***\nYour state has zero neutrons in it :(\n***" << std::endl;
  }
  // else do the things below
    // for each neutron in m_neutrons:
    for (int i = 0; i < m_particles_count; ++i)
    {
      // std::cout<< i<<std::endl;
      //while alive:
      while (!m_particles[i].getIsDead())
      {
        // sample distance to interaction
        std::string mtype = mat->matFinder(m_particles[i].getPos());
        m_particles[i].setMat(mtype);
        double distance = mat->getDist(mtype, m_particles[i].getE(),rgen->getNormRand());
        std::pair<double,double> newpos = m_particles[i].getSteppedPos(distance);
        std::string newmtype = mat->matFinder(newpos);
        // if boundary is being crossed:
        if(newmtype == "void")
        {
          m_particles[i].stepNewPos(distance);
          mtype="void";
          m_particles[i].setMat(mtype);
        }
        if (mtype != newmtype)
        {
          // move to boundary
          std::pair<double,double> oldP = m_particles[i].getPos();
          m_particles[i].setPos(mat->findBound(m_particles[i],distance));
          std::pair<double,double> newP = m_particles[i].getPos();
          double forceD = std::pow(std::pow(oldP.first-newP.first,2)+std::pow(oldP.second-newP.second,2),0.5);
          m_particles[i].addStep(mtype,forceD); //needed to track each step for estimator 

          // update material
          // mtype = mat->matFinder(m_particles[i].getPos());
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
        else if(mtype==newmtype) // some type of collision will happen
        {
            m_particles[i].addCol(mtype,m_particles[i].getE());
            // sample reaction type
            std::string rx = mat->getReactionType(rgen->getNormRand(), mtype, m_particles[i].getE());
            // if absorbed:
            if (rx == "abs")
            {
              // kill neutron
              m_particles[i].killN(1);
              m_particles[i].addAbs(mtype,m_particles[i].getE());
              // tally?
            }
            // if fissioned:
            else if (rx == "fis")
            {
              // kill neutron
              m_particles[i].killN(2);
              m_particles[i].addAbs(mtype,m_particles[i].getE());
              // sample number of neutrons produced & save for next gen
              fish->setFissionSite(m_particles[i], mat->getFisInfo(mtype, rgen->getNormRand()));
            }
            // if scattered:
            else if (rx == "scat")
            {
              // sample new energy and angle
              m_particles[i].setE(mat->getScatEn(m_particles[i].getE(), rgen->getNormRand(), mtype));
              m_particles[i].setAngle(rgen->getNormRand()*4*std::acos(0.0));
            }
            else
            {
              std::cout << "the reaction type isn't matching abs, scatter, or fission. weird!" << std::endl;
            }
        } // end collisions   
      } // end while alive
    } // end for each neutron
    // end of one cycle == end of step
    // std::cout<< "ended step"<<std::endl;
  return m_particles;
}// end of state::step
