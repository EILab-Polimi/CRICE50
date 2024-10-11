#ifndef CLIMATE__H
#define CLIMATE__H

#include <fstream>
#include <cmath>

// ====  Abstract Climate class ========

class Climate{
public:
	Climate();
	~Climate();
	double* statesVector;
	double* toCarbon;
	double* toEcon;
	double* fromCarbon;
	double* fromEcon;
	double* tatm;  	// atmospheric temperature (°C w.r.t 1900)
	int t;			// time step
	virtual void nextStep() = 0;
	virtual void writeHeader(std::fstream& output) = 0;
	virtual void writeStep(std::fstream& output) = 0;
	virtual double* getStates() = 0;
	virtual int getNStates() = 0;
	virtual void updateLinks() = 0;
	virtual void climateDelete() = 0;
	virtual void setECS(double ECS) = 0;
	virtual double getECS()=0;
	virtual double getTCR()=0;
	virtual void sampleUnc() = 0;
	virtual void reset() = 0;
};


// ====  WITCH-CO2-Climate module ========

struct paramsWITCHClimate{
	double sigma1;
	double sigma2;
	double lambda;
	double heat_ocean;
};

class WITCHClimate: public Climate{
public:
	WITCHClimate();
	~WITCHClimate();
	WITCHClimate(int hrzn);
	double* tocean;		// ocean temperature (°C w.r.t 1900)
	double forc;
	paramsWITCHClimate params;
	void readParams();
	void nextStep();
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	double* getStates();
	int getNStates();
	void updateLinks();
	void climateDelete();
	void setECS(double ECS);
	double getECS();
	double getTCR();
	void sampleUnc();
	void reset();
};


// ====  DICE-Climate module ========

struct paramsDICEClimate{
	double c1;
	double c3;
	double c4;
	double fco22x;
	double t2xco2;
};

class DICEClimate: public Climate{
public:
	DICEClimate();
	~DICEClimate();
	DICEClimate(int hrzn);
	double* tocean;		// ocean temperature (°C w.r.t 1900)
	double forc;
	paramsDICEClimate params;
	void readParams();
	void nextStep();
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	double* getStates();
	int getNStates();
	void updateLinks();
	void climateDelete();
	void setECS(double ECS);
	double getECS();
	double getTCR();
	void sampleUnc();
	void reset();
};

// ====  Geoffroy et al. (2013)-Climate module ========
// used in Hansel et al. (2020)

struct paramsGeoffroyClimate{
	double nu;
	double delta_temp;
	double xi1;
	double xi3;
	double xi4;
	double kappa;
	double xi2;
};

class GeoffroyClimate: public Climate{
public:
	GeoffroyClimate();
	~GeoffroyClimate();
	GeoffroyClimate(int hrzn);
	double* tocean;		// ocean temperature (°C w.r.t 1900)
	double forc;
	paramsGeoffroyClimate params;
	void readParams();
	void nextStep();
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	double* getStates();
	int getNStates();
	void updateLinks();
	void climateDelete();
	void setECS(double ECS);
	double getECS();
	double getTCR();
	void sampleUnc();
	void reset();
};

// ====  FAIR temperature model ========
struct FAIRTempParams{
	int horizon;
	double TCR;
	double ECS;
	double f2x;
	double ds;
	double df;
	double tcr_dbl;
	double ks;
	double kf;
	double qs;
	double qf;
	double eff;
	double alpha_tcrecs[2]; 
	double root_delta[2][2]; 
	double stdev; 
};

class FAIRTemp: public Climate{
public:
	FAIRTemp();
	~FAIRTemp();
	FAIRTemp(int hrzn);
	double* ts;
	double* tf;
	double* gmst;
	double* noise;
	FAIRTempParams params;
	void calculate_q();
	void sampleTCRECS();
	void sampleUnc();
	void reset();
	void nextStep();
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	double* getStates();
	int getNStates();
	void updateLinks();
	void climateDelete();
	void setECS(double ECS);
	double getECS();
	double getTCR();
};


#endif