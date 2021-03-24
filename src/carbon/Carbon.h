#ifndef CARBON__H
#define CARBON__H

#include <math.h>
#include <fstream>

// ====  Abstract Carbon class ========

class Carbon{
public:
	Carbon();
	~Carbon();
	double* statesVector;
	double* toClimate;
	double* toEcon;
	double* fromEcon;
	double* fromClimate;
	double* forc; 	// (W/m2) increase w.r.t 1900 - this is the element every carbon component needs to have to pass it to temperature
	int t; 			// time step
	virtual void nextStep() = 0;
	virtual void writeHeader(std::fstream& output) = 0;
	virtual void writeStep(std::fstream& output) = 0;
	virtual double* getStates() = 0;
	virtual int getNStates() = 0;
	virtual void updateLinks() = 0;
	virtual void carbonDelete() = 0;
};


// ====  DICE-Carbon module =======

struct paramsDICECarbon{
	float b11;
	float b21;
	float b12;
	float b22;
	float b32;
	float b23;
	float b33;
	float mateq;
	float mupeq;
	float mloeq;
	float fco22x;
	float fex0;
	float fex1;
};

class DICECarbon: public Carbon{
public:
	DICECarbon();
	~DICECarbon();
	DICECarbon(int hrzn);
	double* mat;		// atmospheric carbon (GtC)
	double* mup;		// upper strata carbon (GtC)
	double* mlo;		// lower strata carbon (GtC)
	double* forcoth; 	// forcing of other GHG (W/m2) - increase w.r.t 1900
	double e;
	paramsDICECarbon params;
	void readParams();
	void nextStep();
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	double* getStates();
	int getNStates();
	void updateLinks();
	void carbonDelete();
};


// ====  WITCH-Carbon module ======

struct paramsWITCHCarbon{
	double CO2toC;
	double at2at;
	double at2up;
	double up2at;
	double up2up;
	double up2lo;
	double lo2up;
	double lo2lo;
	double rfoth_a;
	double rfoth_I;
	double rfc_alpha;
	double rfc_beta;
};

class WITCHCarbon: public Carbon{
public:
	WITCHCarbon();
	~WITCHCarbon();
	WITCHCarbon(int hrzn);
	double* mat;		// atmospheric carbon (GtC)
	double* mup;		// upper strata carbon (GtC)
	double* mlo;		// lower strata carbon (GtC)
	double e;
	paramsWITCHCarbon params;
	void readParams();
	void nextStep();
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	double* getStates();
	int getNStates();
	void updateLinks();
	void carbonDelete();
};


// ====   FAIR-Carbon module ========

struct paramsFAIRCarbon{
	int mateq;
	int mupeq;
	int mloeq;
	double t_scale[4];
	double fraction[4];
	double kappa;
};

class FAIRCarbon: public Carbon{
public:
	FAIRCarbon();
	~FAIRCarbon();
	FAIRCarbon(int hrzn);
	double* alpha;
	double (*c_cycle)[4];
	double* mat;		// atmospheric carbon (GtC)
	double* cca_tot;
	double* forcoth;
	double tatm, e; //current tatm and emissions needed to compute alpha at each time step
	paramsFAIRCarbon params;
	void readParams();
	void computeAlpha();
	void nextStep();
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);
	double* getStates();
	int getNStates();
	void updateLinks();
	void carbonDelete();
};

#endif