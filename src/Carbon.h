#ifndef CARBON__H
#define CARBON__H

class Carbon{
public:
	Carbon();
	double* mat;
	double* mup;
	double* mlo;
	double* forc;
	int t;
	void allocate(int hrzn);
	void nextStep();
	void carbon_delete();
};

#endif