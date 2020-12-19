#ifndef ECON__H
#define ECON__H

#include "EconAgent.h"

struct EconParams{
	double elasmu;
	double prstp;
	double ineqav;
};

class Econ{
public:
	Econ();
	~Econ();
	int agents;					// number of agents	
	EconAgent* agents_ptr;		// pointer to agents
	int t;						// time step
	EconParams params;
	double* e;
	void allocate(int agents, int hrzn);
	void nextStep();
	void econDelete();
};

#endif