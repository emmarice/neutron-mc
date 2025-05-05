#ifndef __EM_H
#define __EM_H

// #include "ke.h"
#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <string>
#include <iostream>
using std::pair;
#include <utility>




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
