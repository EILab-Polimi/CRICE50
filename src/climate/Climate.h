#ifndef CLIMATE__H
#define CLIMATE__H

class Climate{
public:
	Climate();
	~Climate();
	double* tatm; // this is the element every temperature component needs to have to pass it to economy
	virtual void allocate(int hzrn) = 0;
	virtual void nextStep(double forc) = 0;
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
	// double* tatm;		// atmospheric temperature (°C w.r.t 1900)
	double* tocean;		// ocean temperature (°C w.r.t 1900)
	paramsWITCHClimate params;
	int t;				// time step
	void allocate(int hrzn);
	void readParams();
	void nextStep(double forc);
	void climateDelete();
};

////// DICETemp class
struct paramsDICEClimate{
	double sigma1;
	double sigma2;
	double lambda;
	double heat_ocean;
};

class DICEClimate: public Climate{
public:
	DICEClimate();
	~DICEClimate();
	// double* tatm;		// atmospheric temperature (°C w.r.t 1900)
	double* tocean;		// ocean temperature (°C w.r.t 1900)
	paramsDICEClimate params;
	int t;				// time step
	void allocate(int hrzn);
	void readParams();
	void nextStep(double forc);
	void climateDelete();
};

#endif