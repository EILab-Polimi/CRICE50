#ifndef ECON__H
#define ECON__H

#include "EconAgent.h"
#include <fstream>

enum RPMetricType {MEDIAN, MEAN, METRICERR};
enum UtilityType {COOP, NON_COOP, UTILITYERR};
enum GCFTypeSim {GCFSim_N, GCFSim_MIT, GCFSim_MITAD, GCFSim_DAM, GCFSimERR};

struct EconParams{
	double elasmu;
	double prstp;
	double ineqav;
	RPMetricType RPCutoffMetric; // median or mean
	UtilityType utilityType;
	DecisionMakers DMType;
	GCFTypeSim GCFType;
};

class Econ{
public:
	Econ();
	~Econ();
	Econ(int hrzn);
	int agents;					// number of agents	
	EconAgent** agents_ptr;		// pointer to agents
	int t;						// time step
	int horizon;
	EconParams params;
	double* e;
	double* gcf;
	double* GDPpc;
	double* ratio9010;
	double* ratio8020;
	double* statesVector;
	double* cemutotper;
	double* RPCutoff;
	double* globalStates;
	double* toCarbon;
	double* toClimate;
	double* fromCarbon;
	double* fromClimate;
	int nGlobalStates;
	double* econStates;
	double utility;
	double* prctiles;
	void readParams();
	void updateGlobalStates();
	void initializeStates(int numGlobalStates);
	double* getStates();
	int getNStates();
	int getNVars();
	void setEconVariables(double* vars);
	void nextStep();
	void updateLinks();
	void writeHeader(std::fstream& output);
	void writeStep(std::fstream& output);	
	double computeGini();	
	double* computePrctiles();
	double computePrctiles7525();
	void computePrctilesTime(int tidx);
	double computeNET();
	void reset();
	void econDelete();
};

#endif