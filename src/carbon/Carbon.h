#ifndef CARBON__H
#define CARBON__H

class Carbon{
public:
	Carbon();
	double* mat;		// atmospheric carbon (GtC)
	double* mup;		// upper strata carbon (GtC)
	double* mlo;		// lower strata carbon (GtC)
	double* forc;		// forcing
	int t;				// time step
	void allocate(int hrzn);
	void nextStep();
	void carbon_delete();
};

#endif