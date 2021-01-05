#ifndef CLIMATE__H
#define CLIMATE__H

#include<fstream>

class Climate{
public:
	Climate();
	~Climate();
	double* tatm; // this is the element every temperature component needs to have to pass it to economy
	virtual void nextStep(double forc) = 0;
	virtual void writeHeader(std::fstream& output) = 0;
	virtual void writeStep(std::fstream& output) = 0;
	virtual void climateDelete() = 0;
};

////// WITCHTemp class
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
	// double* tatm;		// atmospheric temperature (°C w.r.t 1900)
	double* tocean;		// ocean temperature (°C w.r.t 1900)
	paramsWITCHClimate params;
	int t;				// time step
	void readParams();
	void nextStep(double forc);
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	void climateDelete();
};

////// DICETemp class
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
	// double* tatm;		// atmospheric temperature (°C w.r.t 1900)
	double* tocean;		// ocean temperature (°C w.r.t 1900)
	paramsDICEClimate params;
	int t;				// time step
	void readParams();
	void nextStep(double forc);
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	void climateDelete();
};

#endif