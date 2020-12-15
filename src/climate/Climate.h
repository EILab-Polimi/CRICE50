#ifndef CLIMATE__H
#define CLIMATE__H

struct params{
	double sigma1;
	double sigma2;
	double lambda;
	double heat_ocean;
};

class Climate{
public:
	Climate();
	double* tatm;		// atmospheric temperature (°C w.r.t 1900)
	double* tocean;		// ocean temperature (°C w.r.t 1900)
	params params;
	int t;				// time step
	void allocate(int hrzn);
	void readParams();
	void nextStep(double forc);
	void climate_delete();
};

#endif