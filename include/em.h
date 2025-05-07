#ifndef __EM_H
#define __EM_H

#include "ke.h"
#include "jh.h"
#include "baseClasses.h"
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

class stepper
{
private:
  state m_state;
  int m_particles_count;
  std::vector<neutron> m_particles;
public:
  stepper();
  stepper(state a_state);
  void setState(state a_state);
  std::vector<neutron> step(materialManager * mat, randomGen * rgen, MCstats * fish);
};

#endif
