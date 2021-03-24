#include "Climate.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// constructor
Climate::Climate(){

}
// destructor
Climate::~Climate(){

}


// ====  WITCH-CO2-Climate module ========

// constructor
WITCHClimate::WITCHClimate(){

}
// destructor
WITCHClimate::~WITCHClimate(){

}// allocates memory for the climate component
WITCHClimate::WITCHClimate(int hrzn){
	tatm = new double[hrzn + 1];
	tocean = new double[hrzn + 1];
	statesVector = new double[2];
	toCarbon = new double[1];
	toEcon = tatm;
	t = 0;
	readParams();
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void WITCHClimate::readParams(){
	std::fstream in;
	std::string line;
	in.open("./data_ed57/data_climate_witch/tempc.csv");
		if (!in){
		std::cout << "The tempc file could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[2];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[0].compare("heat_ocean")){
			params.heat_ocean = stod(splitline[1]);
		} 
		else if (!splitline[0].compare("lambda")){
			params.lambda = stod(splitline[1]);
		} 
		else if (!splitline[0].compare("sigma1")){
			params.sigma1 = stod(splitline[1]);
		} 
		else if (!splitline[0].compare("sigma2")){
			params.sigma2 = stod(splitline[1]);
		} 
	}
	in.close();
	tatm[0] = 0.85;
	tocean[0] = 0.0068;
	return;
}
// set ECS value
void WITCHClimate::setECS(double ECS){
	params.lambda = 4.1 / ECS;
	return;
}

double WITCHClimate::getECS(){
	return 4.1/params.lambda;
}

double WITCHClimate::getTCR(){
	return 1.70;
}

// simulates one time step
void WITCHClimate::nextStep(){

	forc = fromCarbon[0];
	// Global temperature increase from pre-industrial levels
	tatm[t+1] = tatm[t] +
		params.sigma1 * (forc - params.lambda * tatm[t]
						 - params.sigma2 * (tatm[t] - tocean[t]));
	
	// Ocean temperature
	tocean[t+1] = tocean[t] + params.heat_ocean * (tatm[t] - tocean[t]);
	updateLinks();
	t++;
	return;
}
void WITCHClimate::writeHeader(std::fstream& output){
	output << "TATM" << "\t" <<
		"TOCEAN" << "\t";
	t = 0;
}
//writes step to output
void WITCHClimate::writeStep(std::fstream& output){
	output << tatm[t] << "\t" <<
		tocean[t] << "\t" ;
	t++;
}
//get states
double* WITCHClimate::getStates(){
	statesVector[0] = tatm[t];
	statesVector[1] = tocean[t];
	return statesVector;
}
void WITCHClimate::updateLinks(){
	toCarbon[0] = tatm[t+1];
	return;
}
// get number of states
int WITCHClimate::getNStates(){
	return 2;
}
// frees allocated memory
void WITCHClimate::climateDelete(){
	delete[] tatm;
	delete[] tocean;
	delete[] statesVector;
	delete[] toCarbon;
	return;
}

void WITCHClimate::sampleUnc(){
	return;
}

void WITCHClimate::reset(){
	t = 0;
	toCarbon[0] = tatm[0];
	return;
}


// ====  DICE-Climate module ========

// constructor
DICEClimate::DICEClimate(){

}
// destructor
DICEClimate::~DICEClimate(){

}// allocates memory for the climate component
DICEClimate::DICEClimate(int hrzn){
	tatm = new double[hrzn + 1];
	tocean = new double[hrzn + 1];
	statesVector = new double[2];
	toCarbon = new double[1];
	toEcon = tatm;
	t = 0;
	readParams();
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void DICEClimate::readParams(){
	std::fstream in;
	std::string sJunk = "";
	in.open("./settings/DICEclimateParams.txt", std::ios_base::in);
	if (!in){
		std::cout << "The DICE climate settings file could not be found!" << std::endl;
	    exit(1);
	}
	while (sJunk!="c1"){
		in >>sJunk;
	}
	in >> params.c1;
	while (sJunk!="c3"){
		in >>sJunk;
	}
	in >> params.c3;
	while (sJunk!="c4"){
		in >>sJunk;
	}
	in >> params.c4;
	while (sJunk!="fco22x"){
		in >>sJunk;
	}
	in >> params.fco22x;
	while (sJunk!="t2xco2"){
		in >>sJunk;
	}
	in >> params.t2xco2;
	while (sJunk!="tatm0"){
		in >>sJunk;
	}
	in >> tatm[0];
	while (sJunk!="tocean0"){
		in >>sJunk;
	}
	in >> tocean[0];
	in.close();
	return;
}
// set ECS value
void DICEClimate::setECS(double ECS){
	params.t2xco2 = ECS;
	return;
}

double DICEClimate::getECS(){
	return params.t2xco2;
}

double DICEClimate::getTCR(){
	return 1.70;
}

// simulates one time step
void DICEClimate::nextStep(){

	forc = fromCarbon[0];
	tatm[t+1] = tatm[t] +
		params.c1 * (forc - params.fco22x / params.t2xco2 * tatm[t] +
						 - params.c3 * (tatm[t] - tocean[t]));
	tocean[t+1] = tocean[t] + params.c4 * (tatm[t] - tocean[t]);
	updateLinks();
	t++;
	return;
}
void DICEClimate::writeHeader(std::fstream& output){
	output << "TATM" << "\t" <<
		"TOCEAN" << "\t";
	t = 0;
}
//writes step to output
void DICEClimate::writeStep(std::fstream& output){
	output << tatm[t] << "\t" <<
		tocean[t] << "\t" ;
	t++;
}
//get states
double* DICEClimate::getStates(){
	statesVector[0] = tatm[t];
	statesVector[1] = tocean[t];
	return statesVector;
}
void DICEClimate::updateLinks(){
	toCarbon[0] = tatm[t+1];
	return;
}
// get number of states
int DICEClimate::getNStates(){
	return 2;
}
// frees allocated memory
void DICEClimate::climateDelete(){
	delete[] tatm;
	delete[] tocean;
	delete[] statesVector;
	delete[] toCarbon;
	return;
}

void DICEClimate::sampleUnc(){
	return;
}

void DICEClimate::reset(){
	t = 0;
	toCarbon[0] = tatm[0];
	return;
}


// ====  Geoffroy et al. (2013) Climate module ========

// constructor
GeoffroyClimate::GeoffroyClimate(){

}
// destructor
GeoffroyClimate::~GeoffroyClimate(){

}// allocates memory for the climate component
GeoffroyClimate::GeoffroyClimate(int hrzn){
	tatm = new double[hrzn + 1];
	tocean = new double[hrzn + 1];
	statesVector = new double[2];
	toCarbon = new double[1];
	toEcon = tatm;
	t = 0;
	readParams();
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void GeoffroyClimate::readParams(){
	std::fstream in;
	std::string sJunk="";
	in.open("./settings/GeoffroyClimateParams.txt");
		if (!in){
		std::cout << "The Geoffroy climate settings file could not be found!" << std::endl;
	    exit(1);
	}
	while (sJunk!="nu"){
		in >>sJunk;
	}
	in >> params.nu;
	while (sJunk!="delta_temp"){
		in >>sJunk;
	}
	in >> params.delta_temp;
	while (sJunk!="xi1"){
		in >>sJunk;
	}
	in >> params.xi1;
	while (sJunk!="xi3"){
		in >>sJunk;
	}
	in >> params.xi3;
	while (sJunk!="xi4"){
		in >>sJunk;
	}
	in >> params.xi4;
	while (sJunk!="kappa"){
		in >>sJunk;
	}
	in >> params.kappa;
	params.xi2 = params.kappa / params.nu;
	while (sJunk!="TLO0"){
		in >>sJunk;
	}
	in >> tocean[0];
	while (sJunk!="TAT0"){
		in >>sJunk;
	}
	in >> tatm[0];
	in.close();
	return;
}
// set ECS value
void GeoffroyClimate::setECS(double ECS){
	params.xi2 = params.kappa / ECS;
	return;
}

double GeoffroyClimate::getECS(){
	return params.nu;
}

double GeoffroyClimate::getTCR(){
	return 1.70;
}
// simulates one time step
void GeoffroyClimate::nextStep(){

	forc = fromCarbon[0];
	double tatm_short = tatm[t];
	for (int tidx=0; tidx < 5; tidx++){
		tatm_short = tatm_short + 1.0/params.xi1 * 
			((forc - params.xi2*tatm_short) + \
			- params.xi3 * (tatm_short - tocean[t]));
	}
	// Global temperature increase from pre-industrial levels
	tatm[t+1] = tatm_short;

	// Ocean temperature
	tocean[t+1] = tocean[t] + \
		5.0 * params.xi3/params.xi4 * (tatm[t] - tocean[t]);
	updateLinks();
	t++;
	return;
}
void GeoffroyClimate::writeHeader(std::fstream& output){
	output << "TATM" << "\t" <<
		"TOCEAN" << "\t";
	t = 0;
	return;
}
//writes step to output
void GeoffroyClimate::writeStep(std::fstream& output){
	output << tatm[t] << "\t" <<
		tocean[t] << "\t" ;
	t++;
	return;
}
//get states
double* GeoffroyClimate::getStates(){
	statesVector[0] = tatm[t];
	statesVector[1] = tocean[t];
	return statesVector;
}
void GeoffroyClimate::updateLinks(){
	toCarbon[0] = tatm[t+1];
	return;
}
// get number of states
int GeoffroyClimate::getNStates(){
	return 2;
}
// frees allocated memory
void GeoffroyClimate::climateDelete(){
	delete[] tatm;
	delete[] tocean;
	delete[] statesVector;
	delete[] toCarbon;
	return;
}

void GeoffroyClimate::sampleUnc(){
	return;
}

void GeoffroyClimate::reset(){
	t = 0;
	toCarbon[0] = tatm[0];
	return;
}

// ====  FAIR temperature model ========
FAIRTemp::FAIRTemp(){

}

FAIRTemp::~FAIRTemp(){

}
FAIRTemp::FAIRTemp(int hrzn){
	params.horizon = hrzn;
	params.alpha_tcrecs[0] = 0.5543371849882144;
	params.alpha_tcrecs[1] =  1.1512945858642294;
	params.root_delta[0][0] = 0.19695275;
	params.root_delta[0][1] = 0.11014884;
	params.root_delta[1][0] = 0.11014884;
	params.root_delta[1][1] = 0.22981541;
	params.stdev = 0.10729947684829523;
	params.eff = 1.0;

	params.TCR = 1.6;
	params.ECS = 2.75;
	params.TCR = 1.8; // central value in AR6
	params.ECS = 3.0; // central value in AR6
	params.f2x = 3.71;
	params.ds = 239.0;
	params.df = 4.1;
	params.tcr_dbl = log(2) / log(1.01);
	params.ks = 0.0;
	params.kf = 0.0;
	params.qs = 0.0;
	params.qf = 0.0;
	ts = new double[params.horizon+1];
	tf = new double[params.horizon+1];
	gmst = new double[params.horizon+1];
	noise = new double[params.horizon+1];
	tatm = new double[params.horizon/5+1]; //tatm follows the 5 year time step of the rest of the model
	statesVector = new double[1];
	toCarbon = new double[1];
	ts[0] = 0.11759814;
	tf[0] = 1.02697844;
	gmst[0] = ts[0]+tf[0];
	tatm[0] = gmst[0];
	toEcon = tatm;
	toCarbon[0] = tatm[0];
	calculate_q();

	t = 0;

	return;
}

void FAIRTemp::calculate_q(){
	params.ks = 1.0 - (params.ds/params.tcr_dbl)*(1.0 - exp(-params.tcr_dbl/params.ds));
	params.kf = 1.0 - (params.df/params.tcr_dbl)*(1.0 - exp(-params.tcr_dbl/params.df));
	params.qs = (1.0/params.f2x) * (1.0/(params.ks - params.kf)) * (params.TCR - params.ECS * params.kf);
	params.qf = (1.0/params.f2x) * (1.0/(params.ks - params.kf)) * (params.ECS * params.ks - params.TCR);
	return;
}

void FAIRTemp::sampleUnc(){
	sampleTCRECS();
	return;
}

void FAIRTemp::sampleTCRECS(){
	// sample normal;
    double norm0 = std::max( -3.0,std::min( 3.0, 
		sqrt(-2 * log(rand() * (1.0 / RAND_MAX))) * 
		sin (2 * M_PI * rand() * (1.0 /RAND_MAX)))) ; // sample from standard normal distribution using box-cox method 
    double norm1 = std::max( -3.0, std::min( 3.0, 
		sqrt(-2 * log(rand() * (1.0 / RAND_MAX))) * 
		sin (2 * M_PI * rand() * (1.0 /RAND_MAX)))) ; // sample from standard normal distribution using box-cox method 

	// obtain tcr as :
	params.TCR = exp(params.alpha_tcrecs[0] + params.root_delta[0][0] * norm0 + params.root_delta[0][1] * norm1);
	// obtain ecs as :
	params.ECS = exp(params.alpha_tcrecs[1] + params.root_delta[1][0] * norm0 + params.root_delta[1][1] * norm1);

	calculate_q();
	return;
}

void FAIRTemp::setECS(double ECS){
	params.ECS = ECS;
	params.TCR = 1.8; // central value AR6
	return;
}

double FAIRTemp::getECS(){
	return params.ECS;
}

double FAIRTemp::getTCR(){
	return params.TCR;
}


void FAIRTemp::reset(){
	t = 0;
	toCarbon[0] = tatm[0];
	return;
}

void FAIRTemp::nextStep(){
	double forc;
	forc = fromCarbon[0];
	ts[t+1] = ts[t] * exp(-1.0/params.ds) + params.qs * (1.0 - exp(-1.0/params.ds)) * forc * params.eff;
	tf[t+1] = tf[t] * exp(-1.0/params.df) + params.qf * (1.0 - exp(-1.0/params.df)) * forc * params.eff;
	gmst[t+1] = ts[t+1] + tf[t+1];
	updateLinks();
	if ((t+1)%5==0){
		tatm[(t+1)/5] = gmst[t+1];
	}
	t++;
	return;

}

void FAIRTemp::writeHeader(std::fstream& output){
	output << "TATM" << "\t" ;
	t = 0;
}
//writes step to output - every 5 years to match Econ time step
void FAIRTemp::writeStep(std::fstream& output){
	output << tatm[t/5] << "\t" ;
	t+=5;
}
//get states
double* FAIRTemp::getStates(){
	statesVector[0] = tatm[t/5];
	return statesVector;
}
void FAIRTemp::updateLinks(){
	toCarbon[0] = gmst[t+1];
	return;
}
// get number of states
int FAIRTemp::getNStates(){
	return 1;
}
// frees allocated memory
void FAIRTemp::climateDelete(){
	delete[] tatm;
	delete[] gmst;
	delete[] ts;
	delete[] tf;
	delete[] noise;
	delete[] statesVector;
	delete[] toCarbon;
	return;
}


// ====  Geoffroy et al. (2013) Climate module ========

// constructor
GeoffroyClimate::GeoffroyClimate(){

}
// destructor
GeoffroyClimate::~GeoffroyClimate(){

}// allocates memory for the climate component
GeoffroyClimate::GeoffroyClimate(int hrzn){
	tatm = new double[hrzn + 1];
	tocean = new double[hrzn + 1];
	statesVector = new double[2];
	toCarbon = new double[1];
	toEcon = tatm;
	t = 0;
	readParams();
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void GeoffroyClimate::readParams(){
	std::fstream in;
	std::string sJunk="";
	in.open("./settings/GeoffroyClimateParams.txt");
		if (!in){
		std::cout << "The Geoffroy climate settings file could not be found!" << std::endl;
	    exit(1);
	}
	while (sJunk!="nu"){
		in >>sJunk;
	}
	in >> params.nu;
	while (sJunk!="delta_temp"){
		in >>sJunk;
	}
	in >> params.delta_temp;
	while (sJunk!="xi1"){
		in >>sJunk;
	}
	in >> params.xi1;
	while (sJunk!="xi3"){
		in >>sJunk;
	}
	in >> params.xi3;
	while (sJunk!="xi4"){
		in >>sJunk;
	}
	in >> params.xi4;
	while (sJunk!="kappa"){
		in >>sJunk;
	}
	in >> params.kappa;
	params.xi2 = params.kappa / params.nu;
	while (sJunk!="TLO0"){
		in >>sJunk;
	}
	in >> tocean[0];
	while (sJunk!="TAT0"){
		in >>sJunk;
	}
	in >> tatm[0];
	in.close();
	return;
}
// simulates one time step
void GeoffroyClimate::nextStep(){

	forc = fromCarbon[0];
	double tatm_short = tatm[t];
	for (int tidx=0; tidx < 5; tidx++){
		tatm_short = tatm_short + 1.0/params.xi1 * 
			((forc - params.xi2*tatm_short) + \
			- params.xi3 * (tatm_short - tocean[t]));
	}
	// Global temperature increase from pre-industrial levels
	tatm[t+1] = tatm_short;

	// Ocean temperature
	tocean[t+1] = tocean[t] + \
		5.0 * params.xi3/params.xi4 * (tatm[t] - tocean[t]);
	// std::cout << "\t\tWITCH climate evolves to next step:" << std::endl;
	// std::cout << "\t\t" << tatm[t] << "\t" << tocean[t] << std::endl;
	updateLinks();
	t++;
	return;
}
void GeoffroyClimate::writeHeader(std::fstream& output){
	output << "TATM" << "\t" <<
		"TOCEAN" << "\t";
	t = 0;
}
//writes step to output
void GeoffroyClimate::writeStep(std::fstream& output){
	output << tatm[t] << "\t" <<
		tocean[t] << "\t" ;
	t++;
}
//get states
double* GeoffroyClimate::getStates(){
	statesVector[0] = tatm[t];
	statesVector[1] = tocean[t];
	return statesVector;
}
void GeoffroyClimate::updateLinks(){
	toCarbon[0] = tatm[t+1];
	return;
}
// get number of states
int GeoffroyClimate::getNStates(){
	return 2;
}
// frees allocated memory
void GeoffroyClimate::climateDelete(){
	delete[] tatm;
	delete[] tocean;
	delete[] statesVector;
	delete[] toCarbon;
	return;
}
