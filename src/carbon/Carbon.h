#ifndef CARBON__H
#define CARBON__H

#include<math.h>
#include<fstream>
// Abstract Carbon class
class Carbon{
public:
	Carbon();
	~Carbon();
	double* forc; // (W/m2) increase w.r.t 1900 - this is the element every carbon component needs to have to pass it to temperature
	virtual void allocate(int hrzn) = 0;
	virtual void nextStep(double e) = 0;
	virtual void carbonDelete() = 0;
};

////// DICECarbon class
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
	double* mat;		// atmospheric carbon (GtC)
	double* mup;		// upper strata carbon (GtC)
	double* mlo;		// lower strata carbon (GtC)
	double* forcoth; 	// forcing of other GHG (W/m2) - increase w.r.t 1900
	paramsDICECarbon params;
	int t;				// time step
	void allocate(int hrzn);
	void readParams();
	void nextStep(double e);
	void carbonDelete();
};

////// WITCHCarbon class
struct paramsWITCHCarbon{
	int param1;
	int param2;
};

class WITCHCarbon: public Carbon{
public:
	WITCHCarbon();
	~WITCHCarbon();
	double* mat;		// atmospheric carbon (GtC)
	double* mup;		// upper strata carbon (GtC)
	double* mlo;		// lower strata carbon (GtC)
	paramsWITCHCarbon params;
	int t;				// time step
	void allocate(int hrzn);
	void nextStep(double e);
	void carbonDelete();
};

// ////// FAIRCarbon class
// struct paramsFAIRCarbon{
// 	int param1;
// 	int param2;
// };

// class FAIRCarbon: public Carbon{
// public:
// 	FAIRCarbon();
// 	~FAIRCarbon();
// 	double* mat;		// atmospheric carbon (GtC)
// 	double* mup;		// upper strata carbon (GtC)
// 	double* mlo;		// lower strata carbon (GtC)
// 	paramsFAIRCarbon params;
// 	int t;				// time step
// 	void allocate(int hrzn);
// 	void nextStep();
// 	void carbon_delete();
// };

#endif