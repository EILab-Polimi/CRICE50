#ifndef CLIMATE__H
#define CLIMATE__H

class Climate{
public:
	Climate();
	double* tatm;
	double* tocean;
	int t;
	void allocate(int hrzn);
	void nextStep();
	void climate_delete();
};

#endif