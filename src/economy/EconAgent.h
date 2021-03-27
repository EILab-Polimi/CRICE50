#ifndef ECONAGENT__H
#define ECONAGENT__H
#include <string>
#include <fstream>
#include "../moeaframework/param_function.h"

enum DamagesType {NO, BURKESR, BURKELR, 
	BURKESR_DIFF, BURKELR_DIFF, 
	DJO, KAHN, DAMAGEERR};
enum OmegaEq {FULL, SIMPLE, OMEGAERR};
enum RPCutoffIndType {BASEGDP, GDP, RPINDERR};
enum TempLimit {ON, OFF, TEMPLIMITERR};
enum ElandType {ELANDBAU, ELANDOPT, ELANDERR};
enum DecisionMakers {BAU, INPUT_STATIC, INPUT_POLICY, DMERR};
enum Adaptation {NOADAPT, ADWITCH, ADAPTERR};

class EconAgent{
public:
	EconAgent();
	virtual ~EconAgent() = 0;
	double utility;
	double* globalStates;
	int nGlobalStates;
	int t; 			// time step
	virtual double getValueForRPCutoff() = 0;
	virtual double getEmissions(int tidx) = 0;
	virtual double getPop(int tidx) = 0;
	virtual double getCPC(int tidx) = 0;
	virtual int getNVars() = 0;
	virtual	void setBAUDMType() = 0;
	virtual void nextStep(double* tatm, double RPCutoff) = 0;
	virtual void setAgentVariables(double* vars) = 0;
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
	OmegaEq omegaEq;
	RPCutoffIndType indRPCutoff;
	TempLimit tempLimit;
	ElandType elandType;
	DecisionMakers DMType;
	Adaptation adaptType;
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
	double dk_adsad;
	double dk_adsac;
	double rho_ad;
	double rho_adact;
	double rho_adcap;
	double miu_ad;
	double phi_ad;
	double beta1_ad;
	double beta2_ad;
	double beta3_ad;
};

struct RICEEconAgentTraj{
	double** pop;
	double** tfp;
	double** gdpbase;
	double** sigma;
	double* gdp;
	double* eind;
	double* e;	
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
	double* adapt;
	double* act;
	double* sad;
	double* fad;
	double* ia;
	double* iac;
	double* ac;
	double* sac;
	double* gac;
	double* adcosts;	
};

struct EconAgentPolicy{
	std::pFunction_param p_param;
	std::param_function* Policy;
	std::vector<double> input, output;
	int nvars;
};

class RICEEconAgent: public EconAgent{
public:
	RICEEconAgent();
	~RICEEconAgent();
	RICEEconAgent(int hrzn, std::string regname, DecisionMakers DMType);
	int horizon;
	int ssp;
	std::string name;
	RICEEconAgentParams params;
	RICEEconAgentTraj traj;
	EconAgentPolicy policy;
	void allocate();
	void readParams();
	void readPolicyParams();
	void readBaseline(int hrzn);
	double getValueForRPCutoff();
	double getEmissions(int tidx);
	double getPop(int tidx);
	double getCPC(int tidx);
	int getNVars();
	void setBAUDMType();
	void nextStep(double* tatm, double RPCutoff);
	void setAgentVariables(double* vars);
	void computeDamages(double RPCutoff);
	void nextAction();
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	void econAgentDelete();
};

#endif