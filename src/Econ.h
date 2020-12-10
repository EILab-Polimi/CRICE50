#ifndef ECON__H
#define ECON__H

#include "EconAgent.h"

class Econ{
public:
	Econ();
	int agents;					// number of agents	
	EconAgent* agents_ptr;		// pointer to agents
	int t;						// time step
	void allocate(int agents, int hrzn);
	void nextStep();
	void econ_delete();
};

#endif