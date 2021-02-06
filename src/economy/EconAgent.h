#ifndef ECONAGENT__H
#define ECONAGENT__H
#include <string>
#include <fstream>
#include "../moeaframework/param_function.h"

enum DamagesType {NO, BURKESR, BURKELR, 
	BURKESR_DIFF, BURKELR_DIFF, 
	DJO, KAHN, COACCH, KALKUHL, DAMAGEERR};
enum OmegaEq {FULL, SIMPLE, OMEGAERR};
enum RPCutoffIndType {BASEGDP, GDP, RPINDERR};
enum TempLimit {ON, OFF, TEMPLIMITERR};
enum ElandType {ELANDBAU, ELANDOPT, ELANDERR};
enum DecisionMakers {BAU, INPUT_STATIC, INPUT_POLICY, DMERR};
enum Adaptation {NOADAPT, ADWITCH, ADAPTERR};
enum Embedding {EMB_YES, EMB_NO, EMBERR};
enum GCFSimType {GCF_YES, GCF_NO, GCFERR};

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
	virtual double getGDPpc(int tidx) = 0;
	virtual double getAbateAdaptCost(int tidx) = 0;
	virtual double getAbateCost(int tidx) = 0;
	virtual double getDamages(int tidx) = 0;
	virtual int getNVars() = 0;
	virtual	void setBAUDMType() = 0;
	virtual void nextStep(double* tatm, double* RPCutoff) = 0;
	virtual void setAgentVariables(double* vars) = 0;
	virtual void writeHeader(std::fstream& output) = 0;
	virtual void writeStep(std::fstream& output) = 0;
	virtual void setSsp(int ssp) = 0;
	virtual void setDamages(int damages) = 0;
	virtual void setAdaptEff(double adapteff) = 0;
	virtual double getGCFFlux(int tidx) = 0;
	virtual void setGCFFlux(double value, int tidx) = 0;
	virtual void reset() = 0;
	virtual void econAgentDelete() = 0;
};

struct RICEEconAgentParams{
	int annual_climate;
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
	int adaptWITCH;
	Embedding embedding;
	GCFSimType GCFSim;
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
	double adapteff;
	double witch_w1;
	double witch_w2;
	double witch_w3;
	double witch_w4;
	double kw_T;
	double kw_DT;
	double kw_DT_lag;
	double kw_TDT;
	double kw_TDT_lag;
	double comega_qmul;
	double comega_b1;
	double comega_b2;
	double comega_c;
	double coacch_temp_base;	
};

struct RICEEconAgentTraj{
	double** pop;
	double** tfp;
	double** gdpbase;
	double** sigma;
	double** eind_bau;
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
	double* rd;
	double* act;
	double* sad;
	double* fad;
	double* ia;
	double* iac;
	double* ac;
	double* sac;
	double* gac;
	double* adcosts;	
	double* gcfFlux;
	double* alpha_gcf;
	double omega_witch;
};

struct EconAgentPolicy{
	std::pFunction_param p_param;
	std::param_function* Policy;
	std::vector<double> input, output;
	std::pFunction_param e_param;
	std::param_function* Embedder;
	std::vector<double> e_input, e_output;
	int nvars, evars;
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
	RICEEconAgent(int hrzn, std::string regname, DecisionMakers DMType, int idx);
	int horizon;
	int ssp;
	std::string name;
	int id_name;
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
	double getGDPpc(int tidx);
	double getAbateAdaptCost(int tidx);
	double getAbateCost(int tidx);
	double getDamages(int tidx);
	int getNVars();
	void setBAUDMType();
	void nextStep(double* tatm, double* RPCutoff);
	void setAgentVariables(double* vars);
	void computeDamages(double* RPCutoff, double* tatm);
	void computeAdaptation(double* tatm);
	void nextAction();
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	void setSsp(int ssps);
	void setDamages(int damages);
	void setAdaptEff(double adapteff);
	void computeEmbedding();
	double getGCFFlux(int tidx) ;
	void setGCFFlux(double value, int tidx);
	void reset();
	void econAgentDelete();
};

#endif