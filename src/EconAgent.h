#ifndef ECONAGENT__H
#define ECONAGENT__H

class EconAgent{
public:
	EconAgent();
	double* k;
	double* y;
	double* l;
	double* tfp;
	int t;
	void allocate(int hrzn);
	void nextStep();
	void econagent_delete();
};

#endif