#ifndef __EM_H
#define __EM_H

#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <pair>
class neutron
{
public:
	double m_energy; // energy in keV
	double m_x; //x position as measured from -- in [units]
	double m_y; //y position as measured from -- in [units]
	double m_theta; //angle in radians
	bool m_isDead;
  int m_DeathType; //0 escape, 1 capture, 2 fission
public:
	neutron();
	~neutron();
	/* void func()
      -brief: 
      -param: None
      -return: No return -- void
  	*/
double getVelocity();
double getE();
void setE(double a_E);
std::pair<double,double> getPos();
void setPos(double a_x,double a_y);
void setPos(std::pair<double,double> a_pos);
double getAngle();
void setAngle(double a_angle);
double getSteppedPos(double a_dist);
void stepNewPos(double a_dist);
void kill(int a_type);

	
};

class state
{
public:
	std::vector<neutron> m_particles;
public:
	state();
	~state();
	void step();
	/* void step()
      -brief: steps forward in time, changing the vector of neutrons for the state.
      -param: None
      -return: No return -- void
  	*/
	std::vector<neutron> getParticles();
  int getNumParticles();
};

class geometry
{
public:
	geometry();
	~geometry();
	
};

// sampling functions

// source guess should be uniform distro in fissionable regions
// less distributed source means more cyles required for convergence
// several 1000s neutrons per cycle (discard first 25 cycles)

// k eigenvalue power method

// shannon entropy
// Pj is fractions of fission sites in a cycle on course mesh
// H = -sum(Pj * ln(Pj))

// plot cyle on x axis
// and keff & H on y
// both should converge before tallying keff and reaction rate

#endif
