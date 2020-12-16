#ifndef TEMP__H
#define TEMP__H

class Temp{
public:
	Temp();
	~Temp();
	double* tatm; // this is the element every temperature component needs to have to pass it to economy
	virtual void allocate(int hzrn) = 0;
	virtual void nextStep(double forc) = 0;
	virtual void temp_delete() = 0;
};

////// WITCHTemp class
struct paramsWITCHTemp{
	double sigma1;
	double sigma2;
	double lambda;
	double heat_ocean;
};

class WITCHTemp: public Temp{
public:
	WITCHTemp();
	~WITCHTemp();
	// double* tatm;		// atmospheric temperature (°C w.r.t 1900)
	double* tocean;		// ocean temperature (°C w.r.t 1900)
	paramsWITCHTemp params;
	int t;				// time step
	void allocate(int hrzn);
	void readParams();
	void nextStep(double forc);
	void temp_delete();
};

////// DICETemp class
struct paramsDICETemp{
	double sigma1;
	double sigma2;
	double lambda;
	double heat_ocean;
};

class DICETemp: public Temp{
public:
	DICETemp();
	~DICETemp();
	// double* tatm;		// atmospheric temperature (°C w.r.t 1900)
	double* tocean;		// ocean temperature (°C w.r.t 1900)
	paramsDICETemp params;
	int t;				// time step
	void allocate(int hrzn);
	void readParams();
	void nextStep(double forc);
	void temp_delete();
};

#endif