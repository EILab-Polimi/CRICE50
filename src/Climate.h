#ifndef CLIMATE__H
#define CLIMATE__H

class Climate{
public:
	Climate();
	double* tatm;		// atmospheric temperature (°C w.r.t 1900)
	double* tocean;		// ocean temperature (°C w.r.t 1900)
	int t;				// time step
	void allocate(int hrzn);
	void nextStep();
	void climate_delete();
};

#endif