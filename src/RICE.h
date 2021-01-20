#ifndef RICE__H
#define RICE__H

#include "./carbon/Carbon.h"
#include "./climate/Climate.h"
#include "./economy/Econ.h"

class RICE{
public:
	RICE();
	~RICE();
	RICE(int hrzn, int carbontype);
	int agents;			// number of economic agents interacting
	int horizon;		// length of the horizon
	int t;				// time instant
	Carbon* carbon;		// pointer to carbon module active in the model
	Climate* climate;	// pointer to climate module active in the model
	Econ* econ;			// pointer to economic module active in the model
	void nextStep();
	void simulate();
	void resetTidx();
	void writeSimulation();
	void RICE_delete();
};

#endif