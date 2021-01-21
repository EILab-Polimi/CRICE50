#ifndef ECON__H
#define ECON__H

#include "EconAgent.h"
#include <fstream>

struct EconParams{
	double elasmu;
	double prstp;
	double ineqav;
	int RPCutoffMetric; // median or mean
};

class Econ{
public:
	Econ();
	~Econ();
	Econ(int hrzn);
	int agents;					// number of agents	
	EconAgent** agents_ptr;		// pointer to agents
	int t;						// time step
	EconParams params;
	double* e;
	void readParams();
	void nextStep(double* tatm);
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);	
	void econDelete();
};

#endif