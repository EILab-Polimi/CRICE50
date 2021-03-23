#include "Carbon.h"

#include <iostream>
#include <sstream>
#include <algorithm>

// constructor
Carbon::Carbon(){

}
// destructor
Carbon::~Carbon(){

}


// =======  DICE-Carbon module =============

// constructor
DICECarbon::DICECarbon(){

}
// destructor
DICECarbon::~DICECarbon(){

}
// allocates the carbon component
DICECarbon::DICECarbon(int hrzn){
	
	mat = new double[hrzn + 1];  // Carbon concentration increase in Atmosphere [GtC from 1750]
	mup = new double[hrzn + 1];  // Carbon concentration increase in Shallow Oceans [GtC from 1750]
	mlo = new double[hrzn + 1];  // Carbon concentration increase in Lower Oceans [GtC from 1750]
	forc = new double[hrzn + 1];  // Increase in Radiative Forcing [W/m2 from 1900]
	forcoth = new double[hrzn + 1];  // Exogenous forcing from other greenhouse gases [W/m2]
	statesVector = new double[3];
	readParams();
	t = 0;
}
// simulates next step
void DICECarbon::nextStep(double e, double* fromClimate){

	// OGHG Forcing
	forcoth[t] = params.fex0 + (params.fex1 - params.fex0) * std::min((double)t/17.0,1.0);
	// Carbon concentration increase in Atmosphere 
	mat[t+1] = mat[t] * params.b11 + mup[t] * params.b21 + e * (5/3.666);
	// Carbon concentration increase in Shallow Oceans 
	mup[t+1] = mat[t] * params.b12 + mup[t] * params.b22 + mlo[t] * params.b32;
	// Carbon concentration increase in Lower Oceans 
	mlo[t+1] = mup[t] * params.b23 + mlo[t] * params.b33;
	// Total radiative forcing
	forc[t] = params.fco22x * ((log((mat[t]/588.000))/log(2))) + forcoth[t];

	// std::cout << "\t\tDICE carbon cycle evolves to next step: " << std::endl;
	// std::cout <<  "\t\t" << mat[t] << "\t" << mup[t] << "\t" << mlo[t] << "\t" << forc[t] << "\t" << t+1 << std::endl;
	t++;
	return;
}

// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void DICECarbon::readParams(){
	std::fstream in;
	std::string sJunk = "";
	in.open("./settings/DICEcarbonParams.txt", std::ios_base::in);
	if (!in){
		std::cout << "The DICE carbon settings file could not be found!" << std::endl;
	    exit(1);
	}
	while (sJunk!="b12"){
		in >>sJunk;
	}
	in >> params.b12;
	while (sJunk!="b23"){
		in >>sJunk;
	}
	in >> params.b23;
	while (sJunk!="mateq"){
		in >>sJunk;
	}
	in >> params.mateq;
	while (sJunk!="mupeq"){
		in >>sJunk;
	}
	in >> params.mupeq;
	while (sJunk!="mloeq"){
		in >>sJunk;
	}
	in >> params.mloeq;
	params.b11 = 1.0 - params.b12;
    params.b21 = params.b12 * params.mateq / params.mupeq;
    params.b22 = 1.0 - params.b21 - params.b23;
    params.b32 = params.b23 * params.mupeq/params.mloeq;
    params.b33 = 1.0 - params.b32 ;
	while (sJunk!="fco22x"){
		in >>sJunk;
	}
	in >> params.fco22x;
	while (sJunk!="fex0"){
		in >>sJunk;
	}
	in >> params.fex0;
	while (sJunk!="fex1"){
		in >>sJunk;
	}
	in >> params.fex1;
	while (sJunk!="mat0"){
		in >>sJunk;
	}
	in >> mat[0];
	while (sJunk!="mup0"){
		in >>sJunk;
	}
	in >> mup[0];
	while (sJunk!="mlo0"){
		in >>sJunk;
	}
	in >> mlo[0];
	in.close();
	return;
}
// Writes header for output
void DICECarbon::writeHeader(std::fstream& output){
	output << "MAT" << "\t" <<
		"MUP" << "\t" <<
		"MLO" << "\t" <<
		"FORC" << "\t" <<
		"FORCOTH" << "\t" ;
	t = 0;
}
// Writes step to output
void DICECarbon::writeStep(std::fstream& output){
	output << mat[t] << "\t" <<
		mup[t] << "\t" <<
		mlo[t] << "\t" <<
		forc[t] << "\t" <<
		forcoth[t] << "\t" ;
	t++;
}
//get states
double* DICECarbon::getStates(){
	statesVector[0] = mat[t];
	statesVector[1] = mup[t];
	statesVector[2] = mlo[t];
	return statesVector;
}
// get number of states
int DICECarbon::getNStates(){
	return 3;
}
// frees allocated memory
void DICECarbon::carbonDelete(){
	delete[] mat;
	delete[] mup;
	delete[] mlo;
	delete[] forc;
	delete[] forcoth;
	delete[] statesVector;
	return;
}





// ====  WITCH-Carbon module =============


// constructor
WITCHCarbon::WITCHCarbon(){

}
// destructor
WITCHCarbon::~WITCHCarbon(){

}
// allocates the carbon component
WITCHCarbon::WITCHCarbon(int hrzn){
	mat = new double[hrzn + 1];  // Carbon concentration increase in Atmosphere [GtC from 1750]
	mup = new double[hrzn + 1];  // Carbon concentration increase in Shallow Oceans [GtC from 1750]
	mlo = new double[hrzn + 1];  // Carbon concentration increase in Lower Oceans [GtC from 1750]
	forc = new double[hrzn + 1];  // Increase in Radiative Forcing [W/m2 from 1900]
	statesVector = new double[3];
	readParams();
	t = 0;
}
// simulates next step
void WITCHCarbon::nextStep(double e, double* fromClimate){

	// Carbon concentration increase in Atmosphere 
	mat[t+1] = mat[t] * params.at2at + mup[t] * params.up2at +
		e * 5 * params.CO2toC;
	// Carbon concentration increase in Shallow Oceans 
	mup[t+1] = mat[t] * params.at2up + mup[t] * params.up2up +
		mlo[t] * params.lo2up;
	// Carbon concentration increase in Lower Oceans 
	mlo[t+1] = mlo[t] * params.lo2lo + mup[t] * params.up2lo;
	// Total radiative forcing
	forc[t] = params.rfoth_I + ( 1 + params.rfoth_a) * 
		(params.rfc_alpha * (log(mat[t]) - log(params.rfc_beta)));


	// std::cout << "\t\tWITCH carbon cycle evolves to next step: " << std::endl;
	// std::cout <<  "\t\t" << mat[t] << "\t" << mup[t] << "\t" << mlo[t] << "\t" << forc[t] << "\t" << t+1 << std::endl;
	t++;
	return;
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void WITCHCarbon::readParams(){
	params.CO2toC = 12.0/44.0;
	std::fstream in;
	std::string line;
	in.open("./data_ed57/data_climate_witch/cmphi.csv");
		if (!in){
		std::cout << "The cmphi file could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[3];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[0].compare("atm")){
			if (!splitline[1].compare("atm")){
				params.at2at = stod(splitline[2]);
			}
			else if (!splitline[1].compare("upp")){
				params.at2up = stod(splitline[2]);
			}
		} 
		else if (!splitline[0].compare("upp")){
			if (!splitline[1].compare("atm")){
				params.up2at = stod(splitline[2]);
			}
			else if (!splitline[1].compare("upp")){
				params.up2up = stod(splitline[2]);
			}
			else if (!splitline[1].compare("low")){
				params.up2lo = stod(splitline[2]);
			}
		} 
		else if (!splitline[0].compare("low")){
			if (!splitline[1].compare("upp")){
				params.lo2up = stod(splitline[2]);
			}
			else if (!splitline[1].compare("low")){
				params.lo2lo = stod(splitline[2]);
			}
		} 
	}
	in.close();
	in.open("./data_ed57/data_climate_witch/rfc.csv");
		if (!in){
		std::cout << "The rfc file could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[3];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[0].compare("co2")){
			if (!splitline[1].compare("alpha")){
				params.rfc_alpha = stod(splitline[2]);
			}
			else if (!splitline[1].compare("beta")){
				params.rfc_beta = stod(splitline[2]);
			}
		} 
	}
	in.close();
	in.open("./data_ed57/data_climate_witch/wcum_emi0.csv");
		if (!in){
		std::cout << "The wcum_emi0 file could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[3];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[0].compare("co2")){
			if (!splitline[1].compare("atm")){
				mat[0] = stod(splitline[2]);
			}
			else if (!splitline[1].compare("upp")){
				mup[0] = stod(splitline[2]);
			}
			else if (!splitline[1].compare("low")){
				mlo[0] = stod(splitline[2]);
			}
		} 
	}
	in.close();
	in.open("./data_ed57/data_climate_witch/oghg_coeff.csv");
		if (!in){
		std::cout << "The oghg_coeff file could not be found!" << std::endl;
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
		if (!splitline[0].compare("intercept")){
			params.rfoth_I = stod(splitline[1]);
		}
		else if (!splitline[0].compare("slope")){
			params.rfoth_a = stod(splitline[1]);
		}
	}
	in.close();
	return;
}
//writes header for output
void WITCHCarbon::writeHeader(std::fstream& output){
	output << "MAT" << "\t" <<
		"MUP" << "\t" <<
		"MLO" << "\t" <<
		"FORC" << "\t" ;
	t = 0;
}
//writes step to output
void WITCHCarbon::writeStep(std::fstream& output){
	output << mat[t] << "\t" <<
		mup[t] << "\t" <<
		mlo[t] << "\t" <<
		forc[t] << "\t" ;
	t++;
}
//get states
double* WITCHCarbon::getStates(){
	statesVector[0] = mat[t];
	statesVector[1] = mup[t];
	statesVector[2] = mlo[t];
	return statesVector;
}
// get number of states
int WITCHCarbon::getNStates(){
	return 3;
}
// frees allocated memory
void WITCHCarbon::carbonDelete(){
	delete[] mat;
	delete[] mup;
	delete[] mlo;
	delete[] forc;
	delete[] statesVector;
	return;
}


// ====   FAIR-Carbon module ========

// constructor
FAIRCarbon::FAIRCarbon(){

}
// destructor
FAIRCarbon::~FAIRCarbon(){

}
// allocates the carbon component
FAIRCarbon::FAIRCarbon(int hrzn){
	mat = new double[hrzn + 1];  // Carbon concentration increase in Atmosphere [GtC from 1750]
	c_cycle = new double[hrzn + 1][4];  // Carbon concentration increase in Atmosphere [GtC from 1750]
	cca_tot = new double[hrzn + 1];
	alpha = new double[hrzn + 1];
	forc = new double[hrzn + 1];  // Increase in Radiative Forcing [W/m2 from 1900]
	forcoth = new double[hrzn + 1];
	statesVector = new double[1];
	readParams();
	std::fstream in;
	in.open("./settings/FAIRforcothSSP2.txt", std::ios_base::in);
	if (!in){
		std::cout << "The FAIR carbon settings file could not be found!" << std::endl;
	    exit(1);
	}
	for (int tidx=0; tidx <= hrzn; tidx++){
		if (tidx <=17){
			in >> forcoth[tidx];			
		}
		else{
			forcoth[tidx] = forcoth[tidx-1];
		}
	}
	in.close();
	t = 0;
}
// simulates next step
void FAIRCarbon::nextStep(double e, double* fromClimate){

	tatm = fromClimate[0];
	// need to get tatm from climate component
	computeAlpha();

	// iterate over boxes
	for (int box=0; box<4; box++){
    	c_cycle[t+1][box] = \
	      c_cycle[t][box] * exp(-5/(alpha[t] * params.t_scale[box])) +
	      params.fraction[box] * (e * 
	      exp(-5/(alpha[t] * params.t_scale[box]))*(1/3.666) ) + 
	      params.fraction[box] * (e * 
	      exp(-(5-1)/(alpha[t] * params.t_scale[box]))*(1/3.666) ) + 
	      params.fraction[box] * (e * 
	      exp(-(5-2)/(alpha[t] * params.t_scale[box]))*(1/3.666) ) + 
	      params.fraction[box] * (e * 
	      exp(-(5-3)/(alpha[t] * params.t_scale[box]))*(1/3.666) ) + 
	      params.fraction[box] * (e * 
	      exp(-(5-4)/(alpha[t] * params.t_scale[box]))*(1/3.666) ) ;
  	}

  	// compute mat
	mat[t+1] = params.mateq;
  	for (int box=0; box < 4; box++){
    	mat[t+1] += c_cycle[t+1][box];
  	}
	mat[t+1] = std::max(1.0, mat[t+1]);
  	
  	// Radiative forcing
	forc[t + 1] = params.kappa * 
    	(log(mat[t + 1] / params.mateq) / log(2.0)) + forcoth[t + 1];

	t++;
	return;
}
void FAIRCarbon::computeAlpha(){
	double input[2];
	//normalize inputs
	input[0] = std::max(0.0, 
    	(cca_tot[t] - (mat[t] - 588.0) - 30.966999999999985) /
    	(620.967 - 30.966999999999985));
	input[1] = std::max(0.0, (tatm - 1.0) / (3.95 - 1.0));

	//compute alpha via ANN
	alpha[t] = 0.4224476755742342 + 
	    (-2.24079509) * 
	    	(-1.0 + 2.0 / 
	        	( 1.0 + exp( -2.0 * 
	          	(-1.1679476786651246 + 
	            	(-0.5497803029711411) * input[0] + 
	            	(-0.6082563253131715) * input[1] )))) +
	    (2.10715655) * 
	    	(-1.0 + 2.0 / 
	        	( 1.0 + exp( -2.0 * 
	        	(-1.9221811095464068 + 
	            	(0.8797355517352923) * input[0] + 
	            	(0.9631872008727567) * input[1] )))) ;

	return;
}
// read parameters from text file
// and stores them in the params struct
// and setting initial conditions
void FAIRCarbon::readParams(){
	std::fstream in;
	std::string sJunk = "";
	in.open("./settings/FAIRCarbonParams.txt", std::ios_base::in);
	if (!in){
		std::cout << "The FAIR carbon settings file could not be found!" << std::endl;
	    exit(1);
	}
	while (sJunk!="MATEQ"){
		in >>sJunk;
	}
	in >> params.mateq;
	while (sJunk!="MUPEQ"){
		in >>sJunk;
	}
	in >> params.mupeq;
	while (sJunk!="MLOEQ"){
		in >>sJunk;
	}
	in >> params.mloeq;
	while (sJunk!="t_scale"){
		in >>sJunk;
	}
	for (int box=0; box<4; box++){
		in >> params.t_scale[box];
	}
	while (sJunk!="fraction"){
		in >>sJunk;
	}
	for (int box=0; box<4; box++){
		in >> params.fraction[box];
	}
	while (sJunk!="kappa"){
		in >>sJunk;
	}
	in >> params.kappa;
	while (sJunk!="mat0"){
		in >>sJunk;
	}
	in >> mat[0];
	while (sJunk!="c_cycle0"){
		in >>sJunk;
	}
	for (int box=0; box<4; box++){
		in >> c_cycle[0][box];
	}
	while (sJunk!="cca_tot0"){
		in >>sJunk;
	}
	in >> cca_tot[0];
	in.close();
	return;
}
//writes header for output
void FAIRCarbon::writeHeader(std::fstream& output){
	output << "MAT" << "\t" <<
		"FORC" << "\t" ;
	t = 0;
}
//writes step to output
void FAIRCarbon::writeStep(std::fstream& output){
	output << mat[t] << "\t" <<
		forc[t] << "\t" ;
	t++;
}
//get states
double* FAIRCarbon::getStates(){
	statesVector[0] = mat[t];
	return statesVector;
}
// get number of states
int FAIRCarbon::getNStates(){
	return 1;
}
// frees allocated memory
void FAIRCarbon::carbonDelete(){
	delete[] mat;
	delete[] c_cycle;
	delete[] alpha;
	delete[] cca_tot;
	delete[] forc;
	delete[] forcoth;
	delete[] statesVector;
	return;
}