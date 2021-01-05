#ifndef ECONAGENT__H
#define ECONAGENT__H
#include<string>
#include<fstream>

class EconAgent{
public:
	EconAgent();
	virtual ~EconAgent() = 0;
	double* e;		//emissions
	virtual void nextStep(double* tatm) = 0;
	virtual void writeHeader(std::fstream& output) = 0;
	virtual void writeStep(std::fstream& output) = 0;
	virtual void econAgentDelete() = 0;
};

struct RICEEconAgentParams{
	double gama;
	double dk;
	double dela0;
	double deland;
	double elasmu;
	double prstp;
	double alpha_tatm;
	double beta_tatm;
	double base_tatm;
	int damage_type;
	double beta_bhm;
	double beta_bhm2;
	double beta_djo;
	double beta_k;
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
	double* i;
	double* ygross;
	double* ynet;
	double* y;
	double* damages;
	double* c;
	double* cpc;
	double* ri;
	double* cprice;
	double* omega;
};

class RICEEconAgent: public EconAgent{
public:
	RICEEconAgent();
	~RICEEconAgent();
	RICEEconAgent(int hrzn, std::string regname);
	int t;			// time step
	int horizon;
	int ssp;
	std::string name;
	RICEEconAgentParams params;
	RICEEconAgentTraj traj;
	void readParams();
	void readBaseline(int hrzn);
	void nextStep(double* tatm);
	void computeDamages(double* tatm);
	void nextAction();
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	void econAgentDelete();
};

#endif