#ifndef RICE__H
#define RICE__H

#include "./carbon/Carbon.h"
#include "./temp/Temp.h"
#include "./economy/Econ.h"

class RICE{
public:
	RICE();
	RICE(int nag, int hrzn);
	int agents;			// number of economic agents interacting
	int horizon;		// length of the horizon
	int t;				// time instant
	Carbon* carbon;		// carbon component of the model
	Temp* temp;	// climate component of the model
	Econ* econ;			// economic component of the model
	void allocate(int nag, int hrzn);
	void nextStep();
	void simulate();
	void RICE_delete();
};

#endif