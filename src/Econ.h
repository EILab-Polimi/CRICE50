#ifndef ECON__H
#define ECON__H

#include "EconAgent.h"

class Econ{
public:
	Econ();
	int agents;
	EconAgent* agents_ptr;
	int t;
	void allocate(int agents, int hrzn);
	void nextStep();
	void econ_delete();
};

#endif