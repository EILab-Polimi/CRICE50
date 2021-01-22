#ifndef ECONAGENT__H
#define ECONAGENT__H
#include <string>
#include <fstream>

enum DamagesType {NO, BURKESR, BURKELR, 
	BURKESR_DIFF, BURKELR_DIFF, 
	DJO, KAHN, DAMAGEERR};
enum RPCutoffIndType {BASEGDP, GDP, RPINDERR};
enum TempLimit {ON, OFF, TEMPLIMITERR};
enum ElandType {ELANDBAU, ELANDOPT, ELANDERR};
enum DecisionMakers {BAU, INPUT_STATIC, INPUT_POLICY, DMERR};

class EconAgent{
public:
	EconAgent();
	virtual ~EconAgent() = 0;
	double* e;		//emissions
	double utility;
	int t; 			// time step
	virtual double getValueForRPCutoff() = 0;
	virtual void nextStep(double* tatm, double RPCutoff) = 0;
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
	double optlr_s;
	double alpha_tatm;
	double beta_tatm;
	double base_tatm;
	DamagesType damagesType;
	RPCutoffIndType indRPCutoff;
	TempLimit tempLimit;
	ElandType elandType;
	DecisionMakers DMType;
	int t_min_miu;
	int t_max_miu;
	double max_miu_up;
	double beta_bhm_sr;
	double beta_bhm_sr_2;
	double beta_bhm_lr;
	double beta_bhm_lr_2;
	double beta_bhm_srdp;
	double beta_bhm_srdp_2;
	double beta_bhm_srdr;
	double beta_bhm_srdr_2;
	double beta_bhm_lrdp;
	double beta_bhm_lrdp_2;
	double beta_bhm_lrdr;
	double beta_bhm_lrdr_2;
	double beta_djo_r;
	double beta_djo_p;
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
	double* miu_up;
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
	double* periodu;
	double* cemutotper;
	double* omega;
	double* tatm_local;
	double* damfrac;
	double* komega;
	double* basegrowthcap;
	double* ynet_estimated;
	double* impact;
};

class RICEEconAgent: public EconAgent{
public:
	RICEEconAgent();
	~RICEEconAgent();
	RICEEconAgent(int hrzn, std::string regname);
	int horizon;
	int ssp;
	std::string name;
	RICEEconAgentParams params;
	RICEEconAgentTraj traj;
	void readParams();
	void readBaseline(int hrzn);
	double getValueForRPCutoff();
	void nextStep(double* tatm, double RPCutoff);
	void computeDamages(double* tatm, double RPCutoff);
	void nextAction();
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	void econAgentDelete();
};

#endif