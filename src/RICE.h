#ifndef RICE__H
#define RICE__H

#include "Carbon.h"
#include "Climate.h"
#include "Econ.h"

class RICE{
public:
	RICE();
	RICE(int nag, int hrzn);
	int agents;
	int horizon;
	int t;
	Carbon carbon;
	Climate climate;
	Econ econ;
	void allocate(int nag, int hrzn);
	void nextStep();
	void simulate();
	void RICE_delete();
};

#endif