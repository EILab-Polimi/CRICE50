#ifndef ECONAGENT__H
#define ECONAGENT__H

struct EconAgentParams{
	double gama;
	double dk;
	double dela0;
	double deland;
};

struct EconAgentTraj{
	double* pop;
	double* tfp;
	double* gdp;
	double* gdpbase;
	double* eindbase;
	double* eind;
	double* k;
	double* mx;
	double* ax;
	double* bx;
	double* eland;
	double* abatecost;
};

class EconAgent{
public:
	EconAgent();
	~EconAgent();
	double* e;		//emissions
	int t;			// time step
	void allocate(int hrzn);
	void nextStep();
	void econAgentDelete();
};

#endif