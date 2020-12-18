#ifndef ECON__H
#define ECON__H

#include "EconAgent.h"

class Econ{
public:
	Econ();
	~Econ();
	int agents;					// number of agents	
	EconAgent* agents_ptr;		// pointer to agents
	int t;						// time step
	double* e;
	void allocate(int agents, int hrzn);
	void nextStep();
	void econDelete();
};

#endif