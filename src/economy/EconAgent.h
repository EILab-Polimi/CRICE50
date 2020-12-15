#ifndef ECONAGENT__H
#define ECONAGENT__H

class EconAgent{
public:
	EconAgent();
	double* k;		// capital stock (trillion USD)
	double* y;		// economic output GDP (trillion USD)
	double* l;		// population (million people)
	double* tfp;	// total factor productivity
	int t;			// time step
	void allocate(int hrzn);
	void nextStep();
	void econagent_delete();
};

#endif