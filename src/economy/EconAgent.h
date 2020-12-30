#ifndef ECONAGENT__H
#define ECONAGENT__H
#include<string>

class EconAgent{
public:
	EconAgent();
	virtual ~EconAgent() = 0;
	double* e;		//emissions
	virtual void nextStep() = 0;
	virtual void econAgentDelete() = 0;
};

struct RICEEconAgentParams{
	double gama;
	double dk;
	double dela0;
	double deland;
};

struct RICEEconAgentTraj{
	double** pop;
	double** tfp;
	double** gdpbase;
	double** sigma;
	double* gdp;
	double* eind;
	double* k;
	double* mx;
	double* ax;
	double* bx;
	double* eland;
	double* abatecost;
	double* miu;
	double* s;
};

class RICEEconAgent: public EconAgent{
public:
	RICEEconAgent();
	~RICEEconAgent();
	RICEEconAgent(int hrzn, std::string regname);
	int t;			// time step
	std::string name;
	RICEEconAgentParams params;
	RICEEconAgentTraj traj;
	void readParams();
	void readBaseline(int hrzn);
	void nextStep();
	void econAgentDelete();
};

#endif