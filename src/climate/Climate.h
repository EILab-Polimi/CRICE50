#ifndef CLIMATE__H
#define CLIMATE__H

#include <fstream>


// ====  Abstract Climate class ========

class Climate{
public:
	Climate();
	~Climate();
	double* statesVector;
	double* tatm;  	// atmospheric temperature (°C w.r.t 1900)
	int t;			// time step
	virtual void nextStep(double forc) = 0;
	virtual void writeHeader(std::fstream& output) = 0;
	virtual void writeStep(std::fstream& output) = 0;
	virtual double* getStates() = 0;
	virtual int getNStates() = 0;
	virtual void climateDelete() = 0;
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
	paramsWITCHClimate params;
	void readParams();
	void nextStep(double forc);
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	double* getStates();
	int getNStates();
	void climateDelete();
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
	paramsDICEClimate params;
	void readParams();
	void nextStep(double forc);
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	double* getStates();
	int getNStates();
	void climateDelete();
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
	paramsGeoffroyClimate params;
	void readParams();
	void nextStep(double forc);
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	double* getStates();
	int getNStates();
	void climateDelete();
};


#endif