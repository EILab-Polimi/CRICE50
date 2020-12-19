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
	Carbon* carbon;		// carbon component of the model
	Climate* climate;	// climate component of the model
	Econ* econ;			// economic component of the model
	void allocate(int hrzn);
	void nextStep();
	void simulate();
	void RICE_delete();
};

#endif