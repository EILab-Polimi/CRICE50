#ifndef ECONAGENT__H
#define ECONAGENT__H

class EconAgent{
public:
	EconAgent();
	virtual ~EconAgent() = 0;
	double* e;		//emissions
	virtual void nextStep() = 0;
	virtual void econAgentDelete() = 0;
};

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

class RICEEconAgent: public EconAgent{
public:
	RICEEconAgent();
	~RICEEconAgent();
	RICEEconAgent(int hrzn);
	int t;			// time step
	void nextStep();
	void econAgentDelete();
};

#endif