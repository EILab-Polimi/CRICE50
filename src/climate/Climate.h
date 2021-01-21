#ifndef CLIMATE__H
#define CLIMATE__H

#include <fstream>


// ====  Abstract Climate class ========

class Climate{
public:
	Climate();
	~Climate();
	double* tatm;  	// atmospheric temperature (°C w.r.t 1900)
	int t;			// time step
	virtual void nextStep(double forc) = 0;
	virtual void writeHeader(std::fstream& output) = 0;
	virtual void writeStep(std::fstream& output) = 0;
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
	void climateDelete();
};


#endif