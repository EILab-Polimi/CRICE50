#include "EconAgent.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <math.h>

DamagesType stringToDamagesType(std::string input){
	if (input == "NO") return NO;
	if (input == "BURKESR") return BURKESR;
	if (input == "BURKELR") return BURKELR;	
	if (input == "BURKESR_DIFF") return BURKESR_DIFF;	
	if (input == "BURKELR_DIFF") return BURKELR_DIFF;	
	if (input == "DJO") return DJO;	
	if (input == "KAHN") return KAHN;	
	return DAMAGEERR;
}
RPCutoffIndType stringToRPCutoffIndType(std::string input){
	if (input == "BASEGDP") return BASEGDP;
	if (input == "GDP") return GDP;	
	return RPINDERR;
}
TempLimit stringToTempLimit(std::string input){
	if (input == "ON") return ON;
	if (input == "OFF") return OFF;	
	return TEMPLIMITERR;
}
ElandType stringToElandType(std::string input){
	if (input == "BAU") return ELANDBAU;
	if (input == "OPT") return ELANDOPT;	
	return ELANDERR;
}
DecisionMakers stringToDecisionMakers(std::string input){
	if (input == "BAU") return BAU;
	if (input == "INPUT_STATIC") return INPUT_STATIC;	
	if (input == "INPUT_POLICY") return INPUT_POLICY;	
	return DMERR;
}

// constructor
EconAgent::EconAgent(){

}
// destructor
EconAgent::~EconAgent(){

}
////// DESCRIPTION OF THE RICE ECONOMIC AGENT //////
// constructor
RICEEconAgent::RICEEconAgent(){

}
// destructor
RICEEconAgent::~RICEEconAgent(){

}
// allocates the memory for an agent
RICEEconAgent::RICEEconAgent(int hrzn, std::string regname){
	e = new double[hrzn + 1];
	t = 0;
	horizon = hrzn;
	name = regname;
	readParams();
	readBaseline(hrzn);
}
void RICEEconAgent::readParams(){
	std::fstream in;
	std::string sJunk = "";
	std::string line;
	in.open("./settings/EconAgentParams.txt", std::ios_base::in);
	if (!in){
		std::cout << "The EconAgentParams settings file could not be found!" << std::endl;
	    exit(1);
	}
	while (sJunk!="gama"){
		in >>sJunk;
	}
	in >> params.gama;
	while (sJunk!="dk"){
		in >>sJunk;
	}
	in >> params.dk;
	while (sJunk!="dela0"){
		in >>sJunk;
	}
	in >> params.dela0;
	while (sJunk!="deland"){
		in >>sJunk;
	}
	in >> params.deland;
	while (sJunk!="ssp"){
		in >>sJunk;
	}
	in >> ssp;
	ssp = ssp - 1; //correct ssp to account for 0 initialization
	while (sJunk!="damages"){
		in >>sJunk;
	}
	in >> line;
	while (sJunk!="RPCutoffInd"){
		in >>sJunk;
	}
	in >> line;
	while (sJunk!="TempLimit"){
		in >>sJunk;
	}
	in >> line;
	while (sJunk!="Eland"){
		in >>sJunk;
	}
	in >> line;
	while (sJunk!="DecisionMakers"){
		in >>sJunk;
	}
	in >> line;
	in.close();
	// THIS HAS TO BE FIXED
	params.damage_type = 1;
	params.temp_limit = 0;
	params.indRPCutoff = 0;
	in.open("./settings/globalEconParams.txt", std::ios_base::in);
	if (!in){
		std::cout << "The Econ settings file could not be found!" << std::endl;
	    exit(1);
	}
	while (sJunk!="elasmu"){
		in >>sJunk;
	}
	in >> params.elasmu;
	while (sJunk!="prstp"){
		in >>sJunk;
	}
	in >> params.prstp;
	in.close();

	// READING FROM CSV FILE
	in.open("./data_ed57/data_climate_regional/climate_region_coef.csv", std::ios_base::in);
	if (!in){
		std::cout << "The climate downscaling parameters could not be found!" << std::endl;
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
		if (!splitline[1].compare(name)){
			if (!(splitline[0]).compare("alpha_temp")){
				params.alpha_tatm = stod(splitline[2]);
			}
			else if (!(splitline[0]).compare("beta_temp")){
				params.beta_tatm = stod(splitline[2]);
			}
			else if (!(splitline[0]).compare("base_temp")){
				params.base_tatm = stod(splitline[2]);
			}			
		}
	}
	in.close();
	params.beta_bhm_sr = 0.0127184;
	params.beta_bhm_sr_2 = -0.0004871;	
	params.beta_bhm_lr = -0.0037497;
	params.beta_bhm_lr_2 = -0.0000955;
	params.beta_bhm_srdp = 0.0254342;
	params.beta_bhm_srdp_2 = -0.000772;
	params.beta_bhm_srdr = 0.0088951;
	params.beta_bhm_srdr_2 = -0.0003155;
	params.beta_bhm_lrdp = -0.0186;
	params.beta_bhm_lrdp_2 = 0.0001513;
	params.beta_bhm_lrdr = -0.0026918;
	params.beta_bhm_lrdr_2 = -0.000022;
	params.beta_djo_r = 0.0261;
	params.beta_djo_p = 0.0261 - 0.01655;
	params.beta_k = -0.0586;
	return;
}
void RICEEconAgent::readBaseline(int hrzn){
	std::fstream in;
	std::string line;
	//carbon intensity
	traj.sigma = new double * [5];
	for (int idxssp=0; idxssp<5; idxssp++){
		traj.sigma[idxssp] = new double[hrzn + 1];
	}
	in.open("./data_ed57/data_baseline/ssp_cintensity.csv");
	if (!in){
		std::cout << "The carbon intensity file specified could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[4];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[2].compare(name) && stoi(splitline[1])<=hrzn){
			char idxssp = splitline[0].back();
			char* idxsspp = &idxssp;
			traj.sigma[atoi(idxsspp)-1][stoi(splitline[1])-1] = stod(splitline[3]);
		}
	}
	in.close();
	//population
	traj.pop = new double * [5];
	for (int idxssp=0; idxssp<5; idxssp++){
		traj.pop[idxssp] = new double[hrzn + 1];
	}
	in.open("./data_ed57/data_baseline/ssp_pop.csv");
	if (!in){
		std::cout << "The pop settings file specified could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[4];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[2].compare(name) && stoi(splitline[1])<=hrzn){
			char idxssp = splitline[0].back();
			char* idxsspp = &idxssp;
			traj.pop[atoi(idxsspp)-1][stoi(splitline[1])-1] = stod(splitline[3]);
		}
	}
	in.close();
	//tfp
	traj.tfp = new double * [5];
	for (int idxssp=0; idxssp<5; idxssp++){
		traj.tfp[idxssp] = new double[hrzn + 1];
	}
	in.open("./data_ed57/data_baseline/ssp_tfp.csv");
	if (!in){
		std::cout << "The tfp settings file could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[4];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[2].compare(name) && stoi(splitline[1])<=hrzn){
			char idxssp = splitline[0].back();
			char* idxsspp = &idxssp;
			traj.tfp[atoi(idxsspp)-1][stoi(splitline[1])-1] = stod(splitline[3]);
		}
	}
	in.close();
	//gdpbase
	traj.gdpbase = new double * [5];
	for (int idxssp=0; idxssp<5; idxssp++){
		traj.gdpbase[idxssp] = new double[hrzn + 1];
	}
	in.open("./data_ed57/data_baseline/ssp_ykali.csv");
	if (!in){
		std::cout << "The gdpbase settings file could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[4];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[2].compare(name) && stoi(splitline[1])<=hrzn){
			char idxssp = splitline[0].back();
			char* idxsspp = &idxssp;
			traj.gdpbase[atoi(idxsspp)-1][stoi(splitline[1])-1] = stod(splitline[3]);
		}
	}
	in.close();
	traj.gdp = new double[hrzn + 1];
	traj.eind = new double[hrzn + 1];
	traj.k = new double[hrzn + 1];
	//initialize k
	in.open("./data_ed57/data_economy/k0.csv");
	if (!in){
		std::cout << "The k0 file could not be found!" << std::endl;
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
		if (!splitline[0].compare(name)){
			traj.k[0] = stod(splitline[1]);
		}
	}
	in.close();
	traj.miu = new double[hrzn + 1];
	traj.s = new double[hrzn + 1];
	//initialize s
	in.open("./data_ed57/data_economy/s0.csv");
	if (!in){
		std::cout << "The s0 file could not be found!" << std::endl;
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
		if (!splitline[0].compare(name)){
			traj.s[0] = stod(splitline[1]);
		}
	}
	in.close();	
	//macc multiplier
	traj.mx = new double[hrzn + 1];
	in.open("./data_ed57/data_macc/mx_multiplier.csv");
	if (!in){
		std::cout << "The mx multiplier file could not be found!" << std::endl;
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
		if (!splitline[1].compare(name) && stoi(splitline[0])<=hrzn){
			traj.mx[stoi(splitline[0])-1] = stod(splitline[2]);
		}
	}
	in.close();
	//macc coefficients
	traj.ax = new double[hrzn + 1];
	traj.bx = new double[hrzn + 1];
	in.open("./data_ed57/data_macc/macc_coeffs.csv");
	if (!in){
		std::cout << "The macc coeffs file could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[4];
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[2].compare(name) && stoi(splitline[1])<=hrzn){
			if (splitline[0]=="a"){
				traj.ax[stoi(splitline[1])-1] = stod(splitline[3]);
			}
			else{
				traj.bx[stoi(splitline[1])-1] = stod(splitline[3]);				
			}
		}
	}
	in.close();
	//land use emissions
	traj.eland = new double[hrzn + 1];
	in.open("./data_ed57/data_land_use/etree_bau.csv");
	if (!in){
		std::cout << "The land use file could not be found!" << std::endl;
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
		if (!splitline[1].compare(name) && stoi(splitline[0])<=hrzn){
			traj.eland[stoi(splitline[0])-1] = stod(splitline[2]);
		}
	}
	in.close();
	traj.abatecost = new double[hrzn + 1];
	traj.i = new double[hrzn + 1];
	traj.ygross = new double[hrzn + 1];
	traj.ynet = new double[hrzn + 1];
	traj.y = new double[hrzn + 1];
	traj.damages = new double[hrzn + 1];
	traj.c = new double[hrzn + 1];
	traj.cpc = new double[hrzn + 1];
	traj.ri = new double[hrzn + 1];
	traj.cprice = new double[hrzn + 1];
	traj.omega = new double[hrzn + 1];
	traj.tatm_local = new double[hrzn + 1];
	traj.damfrac = new double[hrzn + 1];
	traj.komega = new double[hrzn + 1];
	traj.basegrowthcap = new double[hrzn + 1];
	traj.ynet_estimated = new double[hrzn + 1];
	traj.impact = new double[hrzn + 1];
	traj.omega[0] = 0.0;
	return;
}
// returns value for Rich Poor Cutoff
double RICEEconAgent::getValueForRPCutoff(){
	double RPCutoff = 0.0;
	if (params.indRPCutoff == 0){ // statically based on GDP baseline
		RPCutoff = traj.gdpbase[ssp][t];
	}
	if (params.indRPCutoff == 1){ // uses last value of GDP (dynamic)
		RPCutoff = traj.y[t-1];
	}
	return RPCutoff;
}

// simulates one time step
void RICEEconAgent::nextStep(double* tatm, double RPCutoff){
	nextAction();
	// compute ygross
	traj.ygross[t] = traj.tfp[ssp][t] * 
		pow(traj.k[t], params.gama) * 
		pow(traj.pop[ssp][t]/1000.0, 1 - params.gama);
	// compute emissions
	traj.eind[t] = traj.sigma[ssp][t] * 
		traj.ygross[t] * (1 - traj.miu[t]);
	e[t] = traj.eind[t] + traj.eland[t];
	computeDamages(tatm, RPCutoff);	
	// compute abatecost
	traj.abatecost[t] = traj.mx[t] *
		( (traj.ax[t] * pow(traj.miu[t],2) / 2) + 
			(traj.bx[t] * pow(traj.miu[t],5) / 5)) / 1000.0;
	traj.cprice[t] = traj.mx[t] *
		( (traj.ax[t] * pow(traj.miu[t],2)) + 
			(traj.bx[t] * pow(traj.miu[t],4)));
	// compute variables for economic step transition
	traj.ynet[t] = traj.ygross[t] - traj.damages[t];
	traj.y[t] = traj.ynet[t] - traj.abatecost[t];
	traj.i[t] = traj.s[t] * traj.y[t];
	traj.c[t] = traj.y[t] - traj.i[t];
	traj.cpc[t] = 1000 * traj.c[t] / traj.pop[ssp][t];
	// capital stock step transition
	traj.k[t+1] = traj.k[t] * pow(1-params.dk, 5) + 5 * traj.i[t];
	// THIS CAN BE IMPROVED
	if (t >= 1){
		traj.ri[t-1] = (1 + params.prstp) * 
			pow(traj.cpc[t]/traj.cpc[t-1], params.elasmu/5.0) - 1;
	}

	// std::cout << "\t\tHere the region " << name << " evolves to the step " << t+1 << " emitting (GtCO2):" << e[t] << std::endl;
	t++;
	return;
}
// take next action
void RICEEconAgent::nextAction(){
	// set decision variables
	double optlr_s = (params.dk + .004)/(params.dk + .004*params.elasmu + params.prstp)*params.gama;
	traj.miu[t] = std::min(1.0, 0.1 * (double) t);
	traj.miu[t] = 0.0;
	traj.s[t] = traj.s[0] + std::min(1.0, t/57.0) * (optlr_s - traj.s[0]);
	return;	
}
void RICEEconAgent::computeDamages(double* tatm, double RPCutoff){
	// eventually consider 30°C limit
	if (params.temp_limit==1){
		traj.tatm_local[t] = std::min(30.0,params.alpha_tatm + params.beta_tatm * tatm[t]);
	}
	else{
		traj.tatm_local[t] = params.alpha_tatm + params.beta_tatm * tatm[t];	
	}
	if (params.damage_type == 0){
		traj.damages[t] = 0.0;	
		traj.damfrac[t] = 0.0;	
	}
	else{
		if (params.damage_type==1){
			//BURKE - SR
			traj.impact[t] = params.beta_bhm_sr* traj.tatm_local[t] + 
				params.beta_bhm_sr_2* pow(traj.tatm_local[t],2)
				- params.beta_bhm_sr* params.base_tatm 
				- params.beta_bhm_sr_2* pow(params.base_tatm,2);
		}	
		else if (params.damage_type==2){
			//BURKE - LR
			traj.impact[t] = params.beta_bhm_lr* traj.tatm_local[t] + 
				params.beta_bhm_lr_2* pow(traj.tatm_local[t],2)
				- params.beta_bhm_lr* params.base_tatm 
				- params.beta_bhm_lr_2* pow(params.base_tatm,2);
		}
		else if (params.damage_type==3){
			//BURKE - SR diff
			if (params.indRPCutoff==0){
				if (RPCutoff > traj.gdpbase[ssp][t]){
					traj.impact[t] = params.beta_bhm_srdp* traj.tatm_local[t] + 
						params.beta_bhm_srdp_2* pow(traj.tatm_local[t],2)
						- params.beta_bhm_srdp* params.base_tatm 
						- params.beta_bhm_srdp_2* pow(params.base_tatm,2);
				}
				else{
					traj.impact[t] = params.beta_bhm_srdr* traj.tatm_local[t] + 
						params.beta_bhm_srdr_2* pow(traj.tatm_local[t],2)
						- params.beta_bhm_srdr* params.base_tatm 
						- params.beta_bhm_srdr_2* pow(params.base_tatm,2);
				}
			}
			else if (params.indRPCutoff==1){
				if (RPCutoff > traj.y[t-1]){
					traj.impact[t] = params.beta_bhm_srdp* traj.tatm_local[t] + 
						params.beta_bhm_srdp_2* pow(traj.tatm_local[t],2)
						- params.beta_bhm_srdp* params.base_tatm 
						- params.beta_bhm_srdp_2* pow(params.base_tatm,2);
				}
				else{
					traj.impact[t] = params.beta_bhm_srdr* traj.tatm_local[t] + 
						params.beta_bhm_srdr_2* pow(traj.tatm_local[t],2)
						- params.beta_bhm_srdr* params.base_tatm 
						- params.beta_bhm_srdr_2* pow(params.base_tatm,2);
				}
			}
		}
		else if (params.damage_type==4){
			//BURKE - LR diff
			if (params.indRPCutoff==0){
				if (RPCutoff > traj.gdpbase[ssp][t]){
					traj.impact[t] = params.beta_bhm_lrdp* traj.tatm_local[t] + 
						params.beta_bhm_lrdp_2* pow(traj.tatm_local[t],2)
						- params.beta_bhm_lrdp* params.base_tatm 
						- params.beta_bhm_lrdp_2* pow(params.base_tatm,2);
				}
				else{
					traj.impact[t] = params.beta_bhm_lrdr* traj.tatm_local[t] + 
						params.beta_bhm_lrdr_2* pow(traj.tatm_local[t],2)
						- params.beta_bhm_lrdr* params.base_tatm 
						- params.beta_bhm_lrdr_2* pow(params.base_tatm,2);
				}
			}
			else if (params.indRPCutoff==1){
				if (RPCutoff > traj.y[t-1]){
					traj.impact[t] = params.beta_bhm_lrdp* traj.tatm_local[t] + 
						params.beta_bhm_lrdp_2* pow(traj.tatm_local[t],2)
						- params.beta_bhm_lrdp* params.base_tatm 
						- params.beta_bhm_lrdp_2* pow(params.base_tatm,2);
				}
				else{
					traj.impact[t] = params.beta_bhm_lrdr* traj.tatm_local[t] + 
						params.beta_bhm_lrdr_2* pow(traj.tatm_local[t],2)
						- params.beta_bhm_lrdr* params.base_tatm 
						- params.beta_bhm_lrdr_2* pow(params.base_tatm,2);
				}
			}
		}
		else if (params.damage_type==5){
			//DJO
			if (params.indRPCutoff==0){
				if (RPCutoff > traj.gdpbase[ssp][t]){
					traj.impact[t] = params.beta_djo_p * 
						(traj.tatm_local[t] - params.base_tatm); 
				}
				else{
					traj.impact[t] = params.beta_djo_r * 
						(traj.tatm_local[t] - params.base_tatm); 
				}
			}
			else if (params.indRPCutoff==1){
				if (RPCutoff > traj.y[t-1]){
					traj.impact[t] = params.beta_djo_p * 
						(traj.tatm_local[t] - params.base_tatm); 
				}
				else{
					traj.impact[t] = params.beta_djo_r * 
						(traj.tatm_local[t] - params.base_tatm); 
				}			
			}
		}
		else if (params.damage_type==6){
			//KAHN
			double tatm_mavg = 0.0;
			for (int tidx=1; tidx<7; tidx++){
				if (t-tidx < 6){
					tatm_mavg += params.base_tatm;				
				}
				else{
					tatm_mavg += traj.tatm_local[t-tidx];								
				}
			}
			tatm_mavg = tatm_mavg/6.0;
			traj.impact[t] = params.beta_k * 
				(traj.tatm_local[t] - tatm_mavg); 
		}
		traj.komega[t] = pow((traj.k[t] * pow(1 - params.dk , 5) +
			5 * traj.s[t] * traj.tfp[ssp][t] * pow(traj.k[t], params.gama) *
			pow(traj.pop[ssp][t]/1000.0, 1.0 - params.gama) * 1.0 / (1.0 + traj.omega[t]))
			/ traj.k[t], params.gama);
		if (t < horizon - 1){
			traj.basegrowthcap[t] = pow((traj.gdpbase[ssp][t+1]/traj.pop[ssp][t+1])
				/ (traj.gdpbase[ssp][t]/traj.pop[ssp][t]), 1.0/5.0) - 1;
			// EQ OMEGA
			traj.omega[t+1] = (((1.0 + (traj.omega[t])) 
				* (traj.tfp[ssp][t+1]/traj.tfp[ssp][t])
				* pow(traj.pop[ssp][t+1]/traj.pop[ssp][t], 1.0 - params.gama) * traj.pop[ssp][t]/traj.pop[ssp][t+1] 
				* traj.komega[t]
				/ pow(1.0 + traj.basegrowthcap[t] + traj.impact[t], 5) ) - 1.0);
		}
		traj.damfrac[t] = 1.0 - (1.0 / ( 1.0 + traj.omega[t]));
		traj.ynet_estimated[t] = std::min(std::max(traj.ygross[t] 
			* (1 - traj.damfrac[t]), pow(10.0, -4.0) * traj.gdpbase[ssp][t]), 
			2.0 * traj.gdpbase[ssp][t]);
		traj.damages[t] = traj.ygross[t] - traj.ynet_estimated[t];
	}
	return;
}
// writes header
void RICEEconAgent::writeHeader(std::fstream& output){
	output << "POP" << name << "\t" <<
		"TFP" << name << "\t" <<
		"GDPBASE" << name << "\t" <<
		"SIGMA" << name << "\t" <<
		"MX" << name << "\t" <<
		"AX" << name << "\t" <<
		"BX" << name << "\t" <<
		"ELAND" << name << "\t" <<
		"K" << name << "\t" <<
		"YGROSS" << name << "\t" <<
		"MIU" << name << "\t" <<
		"EIND" << name << "\t" <<
		"E" << name << "\t" <<
		"S" << name << "\t" <<
		"I" << name << "\t" <<
		"DAMAGES" << name << "\t" <<
		"YNET" << name << "\t" <<
		"ABATECOST" << name << "\t" <<
		"Y" << name << "\t" <<
		"C" << name << "\t" <<
		"CPC" << name << "\t" <<
		"RI" << name << "\t" <<
		"CPRICE" << name << "\t" <<
		"OMEGA" << name << "\t" << 
		"TATM_LOCAL" << name << "\t" << 
		"DAMFRAC" << name << "\t" << 
		"KOMEGA" << name << "\t" << 
		"BASEGROWTHCAP" << name << "\t" << 
		"YNET_ESTIMATED" << name << "\t" << 
		"IMPACT" << name << "\t" 
			;
	t = 0;
}
// writes timestep
void RICEEconAgent::writeStep(std::fstream& output){
	output << traj.pop[ssp][t] << "\t" <<
		traj.tfp[ssp][t] << "\t" <<
		traj.gdpbase[ssp][t] << "\t" <<
		traj.sigma[ssp][t] << "\t" <<
		traj.mx[t] << "\t" <<
		traj.ax[t] << "\t" <<
		traj.bx[t] << "\t" <<
		traj.eland[t] << "\t" <<
		traj.k[t] << "\t" <<
		traj.ygross[t] << "\t" <<
		traj.miu[t] << "\t" <<
		traj.eind[t] << "\t" <<
		e[t] << "\t" <<
		traj.s[t] << "\t" <<
		traj.i[t] << "\t" <<
		traj.damages[t] << "\t" <<
		traj.ynet[t] << "\t" <<
		traj.abatecost[t] << "\t" <<
		traj.y[t] << "\t" <<
		traj.c[t] << "\t" <<
		traj.cpc[t] << "\t" <<
		traj.ri[t] << "\t" <<
		traj.cprice[t] << "\t" <<
		traj.omega[t] << "\t" <<
		traj.tatm_local[t] << "\t" <<
		traj.damfrac[t] << "\t" <<
		traj.komega[t] << "\t" <<
		traj.basegrowthcap[t] << "\t" <<
		traj.ynet_estimated[t] << "\t" <<
		traj.impact[t] << "\t" 
			;
	t++;
}
// frees allocated memory
void RICEEconAgent::econAgentDelete(){
	for (int idxssp=0; idxssp<5;idxssp++){
		delete[] traj.pop[idxssp];
		delete[] traj.tfp[idxssp];
		delete[] traj.gdpbase[idxssp];
		delete[] traj.sigma[idxssp];
	}
	delete[] traj.pop;
	delete[] traj.eind;
	delete[] traj.tfp;
	delete[] traj.gdpbase;
	delete[] traj.sigma;
	delete[] traj.k;
	delete[] traj.mx;
	delete[] traj.ax;
	delete[] traj.bx;
	delete[] traj.eland;
	delete[] traj.abatecost;
	delete[] traj.miu;
	delete[] traj.s;
	delete[] traj.i;
	delete[] traj.ygross;
	delete[] traj.ynet;
	delete[] traj.y;
	delete[] traj.damages;
	delete[] traj.c;
	delete[] traj.cpc;
	delete[] traj.ri;
	delete[] traj.cprice;
	delete[] e;
	delete[] traj.omega;
	delete[] traj.tatm_local;
	delete[] traj.damfrac;
	delete[] traj.komega;
	delete[] traj.basegrowthcap;
	delete[] traj.ynet_estimated;
	delete[] traj.impact;

	return;
}