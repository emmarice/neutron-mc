#ifndef __EM_H
#define __EM_H

#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

class neutron
{
public:
	double energy; // energy in keV
	double x; //x position as measured from -- in [units]
	double y; //y position as measured from -- in [units]
	double theta; //angle in radians
	bool isDead;
public:
	neutron();
	~neutron();
	/* void func()
      -brief: 
      -param: None
      -return: No return -- void
  	*/
double getVelocity();
	
};

class state
{
public:
	std::vector<neutron> particles;
public:
	state();
	~state();
	void step();
	/* void step()
      -brief: steps forward in time, changing the vector of neutrons for the state.
      -param: None
      -return: No return -- void
  	*/
	
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
