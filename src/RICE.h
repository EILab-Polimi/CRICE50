#ifndef RICE__H
#define RICE__H

#include "./carbon/Carbon.h"
#include "./climate/Climate.h"
#include "./economy/Econ.h"
#include <string>

enum ModelType {WITCH , DICE, FAIR, GEOFFROY, ERR};

class RICE{
public:
	RICE();
	~RICE();
	int agents;						// number of economic agents interacting
	int horizon;					// length of the horizon
	int t;							// time instant
	int nobjs;
	ModelType carbon_model;
	ModelType climate_model;
	int robustness;
	int writefile;
	std::string nameWritefile;
	std::string simScenarios;
	Carbon* carbon;					// pointer to carbon module active in the model
	Climate* climate;				// pointer to climate module active in the model
	Econ* econ;						// pointer to economic module active in the model
	void setVariables(double* vars);
	void createLinks();
	int getNObjs();
	int getNVars();
	void updateGlobalStates();
	void nextStep();
	void simulate();
	void simulateUnc(double* objs);
	void resetTidx();
	void writeSimulation(std::string filename="./simulationOutput.txt");
	void setSsp(int ssp);
	void setDamages(int damages);
	void setECS(double ECS);
	void setAdaptEff(double adapteff);
	void reportObjs(std::string nameSol, double ECS, int ssp, int damages, std::fstream& robustnessOutput);
	void setNameWritefile(std::string nameWfile);
	void RICE_delete();
};

#endif