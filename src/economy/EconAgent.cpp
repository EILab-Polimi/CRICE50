#include "EconAgent.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <math.h>
#include "../moeaframework/rbf.h"
#include "../moeaframework/ann.h"
#include "../moeaframework/pwLinear.h"
#include "../moeaframework/ncRBF.h"
#include "../moeaframework/ann_mo.h"

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
OmegaEq stringToOmegaEq(std::string input){
	if (input == "FULL") return FULL;
	if (input == "SIMPLE") return SIMPLE;
	return OMEGAERR;
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
Adaptation stringToAdaptation(std::string input){
	if (input == "ON") return ADWITCH;
	if (input == "OFF") return NOADAPT;	
	return ADAPTERR;
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
// custom constructor:
// read params, exog. trajectories and allocate
RICEEconAgent::RICEEconAgent(int hrzn, std::string regname, DecisionMakers DMType){
	t = 0;
	horizon = hrzn;
	name = regname;
	params.DMType = DMType;
	allocate();
	readParams();
	if (params.DMType == INPUT_POLICY){
		readPolicyParams();
	}
	readBaseline(hrzn);
}
void RICEEconAgent::allocate(){
	// traj.pop = new double[5][horizon + 1];
	// traj.tfp = new double[5][horizon + 1];
	// traj.gdpbase = new double[5][horizon + 1];
	// traj.sigma = new double[5][horizon + 1];
	// traj.eind_bau = new double[5][horizon + 1];
	traj.gdp = new double[horizon + 1];
	traj.eind = new double[horizon + 1];
	traj.e = new double[horizon + 1];	
	traj.k = new double[horizon + 1];
	traj.mx = new double[horizon + 1];
	traj.ax = new double[horizon + 1];
	traj.bx = new double[horizon + 1];
	traj.eland = new double[horizon + 1];
	traj.abatecost = new double[horizon + 1];
	traj.miu = new double[horizon + 1];
	traj.miu_up = new double[horizon + 1];
	traj.s = new double[horizon + 1];
	traj.i = new double[horizon + 1];
	traj.ygross = new double[horizon + 1];
	traj.ynet = new double[horizon + 1];
	traj.y = new double[horizon + 1];
	traj.damages = new double[horizon + 1];
	traj.c = new double[horizon + 1];
	traj.cpc = new double[horizon + 1];
	traj.ri = new double[horizon + 1];
	traj.cprice = new double[horizon + 1];
	traj.periodu = new double[horizon + 1];
	traj.cemutotper = new double[horizon + 1];
	traj.omega = new double[horizon + 1];
	traj.tatm_local = new double[horizon + 1];
	traj.damfrac = new double[horizon + 1];
	traj.komega = new double[horizon + 1];
	traj.basegrowthcap = new double[horizon + 1];
	traj.ynet_estimated = new double[horizon + 1];
	traj.impact = new double[horizon + 1];
	traj.adapt = new double[horizon + 1];
	traj.rd = new double[horizon + 1];
	traj.act = new double[horizon + 1];
	traj.sad = new double[horizon + 1];
	traj.fad = new double[horizon + 1];
	traj.ia = new double[horizon + 1];
	traj.iac = new double[horizon + 1];
	traj.ac = new double[horizon + 1];
	traj.sac = new double[horizon + 1];
	traj.gac = new double[horizon + 1];	
	traj.adcosts = new double[horizon + 1];	
	return;
}
// read econ agent params
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
	params.damagesType = stringToDamagesType(line);	
	while (sJunk!="omega"){
		in >>sJunk;
	}
	in >> line;
	params.omegaEq = stringToOmegaEq(line);	
	while (sJunk!="RPCutoffInd"){
		in >>sJunk;
	}
	in >> line;
	params.indRPCutoff = stringToRPCutoffIndType(line);	
	while (sJunk!="TempLimit"){
		in >>sJunk;
	}
	in >> line;
	params.tempLimit = stringToTempLimit(line);	
	while (sJunk!="Eland"){
		in >>sJunk;
	}
	in >> line;
	params.elandType = stringToElandType(line);
	while (sJunk!="t_min_miu"){
		in >>sJunk;
	}
	in >> params.t_min_miu;
	while (sJunk!="max_miu_up"){
		in >>sJunk;
	}
	in >> params.max_miu_up;
	while (sJunk!="t_max_miu"){
		in >>sJunk;
	}
	in >> params.t_max_miu;
	while (sJunk!="Adaptation"){
		in >>sJunk;
	}
	in >> line;
	params.adaptType = stringToAdaptation(line);
	in.close();

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
	params.optlr_s = (params.dk + .004)/(params.dk + .004*params.elasmu + params.prstp)*params.gama;

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
	params.beta_djo_r = 0.00261;
	params.beta_djo_p = 0.00261 - 0.01655;
	params.beta_k = -0.0586;
	in.open("./settings/AdaptCoeffOrigED57.txt", std::ios_base::in);
	if (!in){
		std::cout << "The AdaptCoeff settings file could not be found!" << std::endl;
	    exit(1);
	}
	while (sJunk!=name){
		in >>sJunk;
	}
	in >> params.dk_adsad;
	in >> params.dk_adsac;
	in >> params.rho_ad;
	in >> params.rho_adact;
	in >> params.rho_adcap;
	in >> params.miu_ad;
	in >> params.phi_ad;
	in >> params.beta1_ad;
	in >> params.beta2_ad;
	in >> params.beta3_ad;
	in >> traj.gac[0];

	in.close();
	return;
}
void RICEEconAgent::readPolicyParams(){
	std::fstream in;
	std::string sJunk = "";
	in.open("./settings/settingsAgentPolicy.txt", std::ios_base::in);
	if (!in){
		std::cout << "The EconAgentParams settings file could not be found!" << std::endl;
	    exit(1);
	}
	// read policy type (RBF/ANN/...)
	while (sJunk!="<POLICY_CLASS>"){
		in >>sJunk;
	}
	in >> policy.p_param.tPolicy;
	// read input number and bounds
	double i1, i2;
	while (sJunk!="<NUM_INPUT>"){
		in >>sJunk;
	}
	in >> policy.p_param.policyInput;
    //Loop through all of the input data and read in this order:
	for (int i=0; i < policy.p_param.policyInput; i++){
		in >> i1 >> i2;
	    policy.p_param.mIn.push_back(i1);
	    policy.p_param.MIn.push_back(i2);
	}
	if (params.adaptType == ADWITCH){
		policy.p_param.policyInput += 2;
		for (int adaptinput = 0; adaptinput < 2; adaptinput++){
		    policy.p_param.mIn.push_back(0.0);
		    policy.p_param.MIn.push_back(10.0);			
		}
	}
	// read output number and bounds
	double o1, o2;
	while (sJunk!="<NUM_OUTPUT>"){
		in >>sJunk;
	}
	in >> policy.p_param.policyOutput;
    //Loop through all of the input data and read in this order:
    for (int i=0; i < policy.p_param.policyOutput; i++){
    	in >> o1 >> o2;
    	policy.p_param.mOut.push_back(o1);
    	policy.p_param.MOut.push_back(o2);
    }
	if (params.adaptType == ADWITCH){
		policy.p_param.policyOutput += 3;
		for (int adaptoutput = 0; adaptoutput < 3; adaptoutput++){
		    policy.p_param.mOut.push_back(0.0);
		    policy.p_param.MOut.push_back(0.1);			
		}
	}

    // read number of nodes
	while (sJunk!="<POLICY_STRUCTURE>"){
		in >>sJunk;
	}
	in >> policy.p_param.policyStr;
	in.close();

	// allocate policy based on params
	switch (policy.p_param.tPolicy) {
		case 1: // RBF policy
			policy.Policy = new std::rbf(policy.p_param.policyInput,
				policy.p_param.policyOutput,policy.p_param.policyStr);
			break;
		case 2: // ANN
			policy.Policy = new std::ann(policy.p_param.policyInput,
				policy.p_param.policyOutput,policy.p_param.policyStr);
			policy.nvars = policy.p_param.policyOutput * 
				(policy.p_param.policyStr * 
				(policy.p_param.policyInput + 2) + 1); 
			break;
		case 3: // piecewise linear policy
			policy.Policy = new std::pwLinear(policy.p_param.policyInput,
				policy.p_param.policyOutput,policy.p_param.policyStr);
			break;
		case 4:
			policy.Policy = new std::ncRBF(policy.p_param.policyInput,
				policy.p_param.policyOutput,policy.p_param.policyStr);
			policy.nvars = policy.p_param.policyStr * 
				(2 * policy.p_param.policyInput + policy.p_param.policyOutput) 
				+ policy.p_param.policyOutput; 
			break;
		case 5:
			policy.Policy = new std::annmo(policy.p_param.policyInput,
				policy.p_param.policyOutput,policy.p_param.policyStr);
			policy.nvars = policy.p_param.policyStr * 
				(policy.p_param.policyInput + policy.p_param.policyOutput + 1) 
				+ policy.p_param.policyOutput; 
			break;
		default:
			break;
	}
	// save bounds in input and outputs
	policy.Policy->setMaxInput(policy.p_param.MIn); 
	policy.Policy->setMaxOutput(policy.p_param.MOut);
	policy.Policy->setMinInput(policy.p_param.mIn); 
	policy.Policy->setMinOutput(policy.p_param.mOut);	
	policy.input.resize(policy.p_param.policyInput);
	policy.output.resize(policy.p_param.policyOutput);

	return;
}
// return number of variables
int RICEEconAgent::getNVars(){
	int nvars;
	if (params.DMType == INPUT_STATIC){
		if (params.adaptType == ADWITCH){
			nvars = horizon * 5;
		}
		else{
			nvars = horizon * 2;
		}
	} 
	if (params.DMType == INPUT_POLICY){
		nvars = policy.nvars;
	}
	return nvars;
}
// read exogenous baseline trajectories
void RICEEconAgent::readBaseline(int hrzn){
	std::fstream in;
	std::string line;
	//eind_bau
	traj.eind_bau = new double * [5];
	for (int idxssp=0; idxssp<5; idxssp++){
		traj.eind_bau[idxssp] = new double[hrzn + 1];
	}
	in.open("./data_ed57/data_baseline/ssp_eind_bau.csv");
	if (!in){
		std::cout << "The BAU industrial emissions file specified could not be found!" << std::endl;
	    exit(1);
	}
	while(std::getline(in, line)){
		line.erase(std::remove(line.begin(), line.end(), '"'), line.end());
		std::istringstream s(line);
		std::string field;
		std::string splitline[4];
		std::string idxssp;
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[2].compare(name) && stoi(splitline[1])<=hrzn){
			idxssp = splitline[0][3];
			traj.eind_bau[stoi(idxssp)-1][stoi(splitline[1])-1] = stod(splitline[3]);
		}
	}
	in.close();

	// carbon intensity
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
		std::string idxssp;
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[2].compare(name) && stoi(splitline[1])<=hrzn){
			idxssp = splitline[0][3];
			traj.sigma[stoi(idxssp)-1][stoi(splitline[1])-1] = stod(splitline[3]);
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
		std::string idxssp;
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[2].compare(name) && stoi(splitline[1])<=hrzn){
			idxssp = splitline[0][3];
			traj.pop[stoi(idxssp)-1][stoi(splitline[1])-1] = stod(splitline[3]);
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
		std::string idxssp;
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[2].compare(name) && stoi(splitline[1])<=hrzn){
			idxssp = splitline[0][3];
			traj.tfp[stoi(idxssp)-1][stoi(splitline[1])-1] = stod(splitline[3]);
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
		std::string idxssp;
		int count = 0;
		while (std::getline(s, field, ',')){
			splitline[count] = field;
			count++;
		}
		if (!splitline[2].compare(name) && stoi(splitline[1])<=hrzn){
			idxssp = splitline[0][3];
			traj.gdpbase[stoi(idxssp)-1][stoi(splitline[1])-1] = stod(splitline[3]);
		}
	}
	in.close();

	// traj.gdp = new double[hrzn + 1];
	// traj.eind = new double[hrzn + 1];
	// traj.e = new double[hrzn + 1];
	// traj.k = new double[hrzn + 1];
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
	// traj.miu = new double[hrzn + 1];
	// traj.miu_up = new double[hrzn + 1];
	for (int tidx = 0; tidx < horizon ; tidx++){
		traj.miu_up[tidx] = 1.0 + std::max(0.0, std::min(
			(params.max_miu_up - 1.0) * 
			(2015.0 + 5 * tidx - params.t_min_miu) / 
			(params.t_max_miu - params.t_min_miu), 
			params.max_miu_up - 1.0 ));
	}
	// traj.s = new double[hrzn + 1];
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
	// traj.mx = new double[hrzn + 1];
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
	// traj.ax = new double[hrzn + 1];
	// traj.bx = new double[hrzn + 1];
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
	// traj.eland = new double[hrzn + 1];
	switch (params.elandType){
		case ELANDBAU:
			in.open("./data_ed57/data_land_use/etree_bau.csv");
			break;
		case ELANDOPT:
			in.open("./data_ed57/data_land_use/etree_opt.csv");
			break;
		case ELANDERR:
			std::cerr << "Please insert an available option for ELAND" << std::endl;
	}
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
	// traj.abatecost = new double[hrzn + 1];
	// traj.i = new double[hrzn + 1];
	// traj.ygross = new double[hrzn + 1];
	// traj.ynet = new double[hrzn + 1];
	// traj.y = new double[hrzn + 1];
	// traj.damages = new double[hrzn + 1];
	// traj.c = new double[hrzn + 1];
	// traj.cpc = new double[hrzn + 1];
	// traj.ri = new double[hrzn + 1];
	// traj.cprice = new double[hrzn + 1];
	// traj.periodu = new double[hrzn + 1];
	// traj.cemutotper = new double[hrzn + 1];	
	// traj.omega = new double[hrzn + 1];
	traj.omega[0] = 0.0;
	// traj.tatm_local = new double[hrzn + 1];
	// traj.damfrac = new double[hrzn + 1];
	// traj.komega = new double[hrzn + 1];
	// traj.basegrowthcap = new double[hrzn + 1];
	// traj.ynet_estimated = new double[hrzn + 1];
	// traj.impact = new double[hrzn + 1];
	// traj.adapt = new double[hrzn + 1];
	// traj.act = new double[hrzn + 1];
	// traj.sad = new double[hrzn + 1];
	// traj.fad = new double[hrzn + 1];
	// traj.ia = new double[hrzn + 1];
	// traj.iac = new double[hrzn + 1];
	// traj.ac = new double[hrzn + 1];
	// traj.gac = new double[hrzn + 1];
	// traj.sac = new double[hrzn + 1];
	return;
}
// set agent's decision variables
void RICEEconAgent::setAgentVariables(double* vars){
	if (params.DMType == INPUT_STATIC){
		for (int tidx=0; tidx < horizon; tidx++){
			if (tidx > 0) {
				vars += 2;
				traj.miu[tidx] = vars[0];
				traj.s[tidx] = vars[1];
				if (params.adaptType == ADWITCH){
					vars += 3;
					traj.fad[t] = vars[0];
					traj.ia[t] = vars[1];
					traj.iac[t] = vars[2];					
				}
			}
		}
	}
	else if (params.DMType == INPUT_POLICY){
		policy.Policy->clearParameters();
		policy.Policy->setParameters(vars);
	}
	return;
}
// returns value for Rich Poor Cutoff
double RICEEconAgent::getValueForRPCutoff(){
	switch (params.indRPCutoff){
		case BASEGDP:
			return traj.gdpbase[ssp][t];
		case GDP:
			return traj.y[t-1];
		case RPINDERR:
			std::cerr << "Please insert a valid option for RPCutoff" << std::endl;
			exit(1);
	}
}
// sets BAU simulation
void RICEEconAgent::setBAUDMType(){
	params.DMType = BAU;
}
// simulates one time step
void RICEEconAgent::nextStep(double* tatm, double RPCutoff){
	//take action first based on available information 
	// (especially in adaptive decision making setting)
	// eventually consider 30°C limit
	if (params.tempLimit == ON){
		traj.tatm_local[t] = std::min(30.0, params.alpha_tatm + params.beta_tatm * tatm[t]);
	}
	else{
		traj.tatm_local[t] = params.alpha_tatm + params.beta_tatm * tatm[t];	
	}
	nextAction();		

	// compute ygross
	traj.ygross[t] = traj.tfp[ssp][t] * 
		pow(traj.k[t], params.gama) * 
		pow(traj.pop[ssp][t]/1000.0, 1 - params.gama);

	// compute emissions
	traj.eind[t] = traj.sigma[ssp][t] * 
		traj.ygross[t] * (1 - traj.miu[t]);
	traj.e[t] = traj.eind[t] + traj.eland[t];
	computeDamages(RPCutoff);	

	computeAdaptation();

	// compute abatecost
	traj.abatecost[t] = traj.mx[t] *
		( (traj.ax[t] * pow(traj.miu[t],2) / 2) + 
			(traj.bx[t] * pow(traj.miu[t],5) / 5)) *
			traj.eind_bau[ssp][t] / 1000.0;
	traj.cprice[t] = traj.mx[t] *
		( (traj.ax[t] * pow(traj.miu[t],2)) + 
			(traj.bx[t] * pow(traj.miu[t],4)));

	// compute variables for economic step transition
	// traj.ynet[t] = traj.ygross[t] - traj.damages[t];
	// traj.y[t] = std::max(0.01, traj.ynet[t] - traj.abatecost[t]); //avoid damages making negative gdp
	traj.ynet[t] = traj.ygross[t] - traj.rd[t];
	traj.y[t] = std::max(0.01, traj.ynet[t] - traj.abatecost[t] - traj.adcosts[t]); //avoid damages making negative gdp
	traj.i[t] = traj.s[t] * traj.y[t];
	traj.c[t] = traj.y[t] - traj.i[t];
	traj.cpc[t] = 1000 * traj.c[t] / traj.pop[ssp][t];

	// capital stock step transition
	traj.k[t+1] = traj.k[t] * pow(1-params.dk, 5) + 5 * traj.i[t];
	if (t >= 1){	// THIS CAN BE IMPROVED
		traj.ri[t] = (1 + params.prstp) * 
			pow(traj.cpc[t]/traj.cpc[t-1], params.elasmu/5.0) - 1;
	}

	//compute utility per period
	traj.periodu[t] = (pow(traj.cpc[t], 1 - params.elasmu) - 1.0) / 
		(1.0 - params.elasmu) - 1.0;
	traj.cemutotper[t] = traj.pop[ssp][t] * traj.periodu[t] * 
		(1.0 / pow(1.0 + params.prstp, 5*t));
	utility += traj.cemutotper[t];
	// std::cout << "\t\tHere the region " << name << " evolves to the step " << t+1 << " emitting (GtCO2):" << e[t] << std::endl;
	t++;
	return;
}
void RICEEconAgent::computeAdaptation(){
	if (params.adaptType == ADWITCH){
		// traj.fad[t] = 0.03;
		// traj.ia[t] = 0.03;
		// traj.iac[t] = 0.03;
		if (t==0){
			traj.sad[t] = 0.0;
			traj.sac[t] = 0.0;
			traj.fad[t] = 0.0;
			traj.ia[t] = 0.0;
			traj.iac[t] = 0.0;
		}
		// compute adaptation costs
		traj.adcosts[t] = traj.ygross[t] * 
			(traj.fad[t] + traj.ia[t] + traj.iac[t]);
		
		// compute adaptation action
		traj.act[t] = params.beta1_ad * 
			pow(params.beta2_ad * 
			pow(traj.fad[t] * traj.ygross[t], params.rho_adact) + \
			(1 - params.beta2_ad) * 
			pow(traj.sad[t], params.rho_adact), 
			params.beta3_ad / params.rho_adact);
		// compute adaptation capacity
		traj.ac[t] = pow(params.phi_ad * pow(traj.sac[t], params.rho_adcap) + \
			(1 - params.phi_ad) * pow(traj.gac[t], params.rho_adcap), 
			1 / params.rho_adcap);
		// compute adaptation total
		traj.adapt[t] = pow(params.miu_ad * pow(traj.act[t], params.rho_ad) + \
			(1 - params.miu_ad) * pow(traj.ac[t], params.rho_ad), 
			1 / params.rho_ad);
		// compute residual damages
		traj.rd[t] = traj.damages[t] / (1 + traj.adapt[t]);

		// update adaptation stocks (capital and specific capacity)
		traj.sad[t+1] = traj.sad[t] * (1 - params.dk_adsad) + \
			traj.ia[t] * traj.ygross[t];
		traj.sac[t+1] = traj.sac[t] * (1 - params.dk_adsac) + \
			traj.iac[t] * traj.ygross[t];
		traj.gac[t+1] = traj.gac[t] * (traj.tfp[ssp][t+1]/traj.tfp[ssp][t]); //actually exogenous grows with tfp growth rate	
	}
	else{ // without adaptation
		traj.rd[t] = traj.damages[t];
		traj.adcosts[t] = 0.0;
	}
	return;
}
// take next action
void RICEEconAgent::nextAction(){
	// set decision variables
	if (t>0){
		switch (params.DMType){
			case INPUT_POLICY:
				// retrieve states or input to the policy
				policy.input.clear();
				policy.output.clear();
				// inputs: effK, tatm_local, tatm, tocean, mat, mup, mlo, time
				policy.input.push_back(traj.k[t]/(traj.tfp[ssp][t] * traj.pop[ssp][t])); //as in DICE
				// policy.input.push_back(traj.y[t - 1]/(traj.tfp[ssp][t - 1] * traj.pop[ssp][t - 1])); //maybe better (?)
				policy.input.push_back(traj.tatm_local[t]);
				for (int s = 0; s < nGlobalStates; s++){
					policy.input.push_back(globalStates[s]);
				}
				policy.input.push_back(t);
				if (params.adaptType == ADWITCH){
					policy.input.push_back(traj.sad[t] / traj.k[t] * 100.0);
					policy.input.push_back(traj.sac[t] / traj.k[t] * 100.0);
				}
				
				policy.output = policy.Policy->get_NormOutput(policy.input);
				traj.miu[t] = policy.output[0];
				traj.s[t] = policy.output[1];
				if (params.adaptType == ADWITCH){
					traj.fad[t] = policy.output[2];
					traj.ia[t] = policy.output[3];
					traj.iac[t] = policy.output[4];
				}

				// if savings are fixed
				traj.s[t] = traj.s[0] + std::min(1.0, t/57.0) * (params.optlr_s - traj.s[0]);
				break;
			case BAU:
				// traj.miu[t] = std::min(traj.miu_up[t], 0.1 * (double) t);
				traj.miu[t] = 0.0;
				traj.s[t] = traj.s[0] + std::min(1.0, t/57.0) * (params.optlr_s - traj.s[0]);
				if (params.adaptType == ADWITCH){
					traj.fad[t] = 0.0;
					traj.ia[t] = 0.0;
					traj.iac[t] = 0.0;
				}				
				break;
			case INPUT_STATIC:
				// no need to do anything here
				// miu, s & other decs. vars are fixed 
				// at the beginning of simulation
				// std::cerr << "not developed yet" << std::endl;
				// exit(1);
				break;
			case DMERR:
				std::cerr << "Please insert an available option for DMType" << std::endl;
		}
		traj.miu[t] = std::max(0.0, std::min(traj.miu_up[t], std::min(traj.miu[t-1] + 0.2, traj.miu[t])));
		traj.s[t] = std::max(0.001, std::min(0.999, traj.s[t]));
		if (params.adaptType == ADWITCH){
			traj.fad[t] = std::min(0.1, std::max(0.0, traj.fad[t]));
			traj.ia[t] = std::min(0.1, std::max(0.0, traj.ia[t]));
			traj.iac[t] = std::min(0.1, std::max(0.0, traj.iac[t]));			
		}
	}
	else{
		traj.miu[0] = 0.0;
	}
	traj.miu[1] = 0.03;
	// if (t >= horizon - 10){
	// 	traj.s[t] = params.optlr_s; 
	// }
	return;	
}
void RICEEconAgent::computeDamages(double RPCutoff){
	if (params.damagesType == NO){
		traj.damages[t] = 0.0;	
		traj.damfrac[t] = 0.0;	
	}
	else{
		if (params.damagesType == BURKESR){
			//BURKE - SR
			traj.impact[t] = params.beta_bhm_sr* traj.tatm_local[t] + 
				params.beta_bhm_sr_2* pow(traj.tatm_local[t],2)
				- params.beta_bhm_sr* params.base_tatm 
				- params.beta_bhm_sr_2* pow(params.base_tatm,2);
		}	
		else if (params.damagesType == BURKELR){
			//BURKE - LR
			traj.impact[t] = params.beta_bhm_lr* traj.tatm_local[t] + 
				params.beta_bhm_lr_2* pow(traj.tatm_local[t],2)
				- params.beta_bhm_lr* params.base_tatm 
				- params.beta_bhm_lr_2* pow(params.base_tatm,2);
		}
		else if (params.damagesType == BURKESR_DIFF){
			//BURKE - SR diff
			if (params.indRPCutoff == BASEGDP){
				if (RPCutoff[t] >= traj.gdpbase[ssp][t] / traj.pop[ssp][t]){
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
			else if (params.indRPCutoff == GDP){
				if (RPCutoff[t] >= traj.y[t-1] / traj.pop[ssp][t]){
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
		else if (params.damagesType == BURKELR_DIFF){
			//BURKE - LR diff
			if (params.indRPCutoff == BASEGDP){
				if (RPCutoff[t] >= traj.gdpbase[ssp][t] / traj.pop[ssp][t]){
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
			else if (params.indRPCutoff == GDP){
				if (RPCutoff[t] >= traj.y[t-1] / traj.pop[ssp][t]){
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
		else if (params.damagesType == DJO){
			//DJO
			if (params.indRPCutoff == BASEGDP){
				if (RPCutoff[0] >= traj.gdpbase[ssp][0] / traj.pop[ssp][0]){
					traj.impact[t] = params.beta_djo_p * 
						(traj.tatm_local[t] - params.base_tatm); 
				}
				else{
					traj.impact[t] = params.beta_djo_r * 
						(traj.tatm_local[t] - params.base_tatm); 
				}
			}
			else if (params.indRPCutoff == GDP){
				if (RPCutoff[t] >= traj.y[t-1] / traj.pop[ssp][t]){
					traj.impact[t] = params.beta_djo_p * 
						(traj.tatm_local[t] - params.base_tatm); 
				}
				else{
					traj.impact[t] = params.beta_djo_r * 
						(traj.tatm_local[t] - params.base_tatm); 
				}			
			}
		}
		else if (params.damagesType == KAHN){
			//KAHN
			double tatm_mavg = 0.0;
			for (int tidx=0; tidx<6; tidx++){
				if (t-tidx < 0){
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
			if (params.omegaEq == FULL){
				traj.omega[t+1] = (((1.0 + (traj.omega[t])) 
					* (traj.tfp[ssp][t+1]/traj.tfp[ssp][t])
					* pow(traj.pop[ssp][t+1]/traj.pop[ssp][t], 1.0 - params.gama) * traj.pop[ssp][t]/traj.pop[ssp][t+1] 
					* traj.komega[t]
					/ pow(1.0 + traj.basegrowthcap[t] + traj.impact[t], 5) ) - 1.0);				
			}
			else if (params.omegaEq == SIMPLE){
				traj.omega[t+1] = (1.0 + traj.omega[t]) / 
					pow(1 + traj.impact[t], 5) - 1;				
			}
		}
		traj.damfrac[t] = 1.0 - (1.0 / ( 1.0 + traj.omega[t]));
		traj.ynet_estimated[t] = std::min(std::max(traj.ygross[t] 
			* (1 - traj.damfrac[t]), pow(10.0, -4.0) * traj.gdpbase[ssp][t]), 
			2.0 * traj.gdpbase[ssp][t]);
		traj.damages[t] = traj.ygross[t] - traj.ynet_estimated[t];	
	}
	return;
}
// get emissions
double RICEEconAgent::getEmissions(int tidx){
	return traj.e[tidx];
}
// get population
double RICEEconAgent::getPop(int tidx){
	return traj.pop[ssp][tidx];
}
// get consumption per capita
double RICEEconAgent::getCPC(int tidx){
	return traj.cpc[tidx];
}
// get gdp per capita
double RICEEconAgent::getGDPpc(int tidx){
	return traj.y[tidx] / traj.pop[ssp][tidx];
}
// writes header
void RICEEconAgent::writeHeader(std::fstream& output){
	output << "POP" << name << "\t" <<
		"TFP" << name << "\t" <<
		"GDPBASE" << name << "\t" <<
		"SIGMA" << name << "\t" <<
		"EINDBAU" << name << "\t" <<
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
		"PERIODU" << name << "\t" <<
		"CEMUTOTPER" << name << "\t" <<
		"OMEGA" << name << "\t" << 
		"TATM_LOCAL" << name << "\t" << 
		"DAMFRAC" << name << "\t" << 
		"KOMEGA" << name << "\t" << 
		"BASEGROWTHCAP" << name << "\t" << 
		"YNET_ESTIMATED" << name << "\t" << 
		"IMPACT" << name << "\t" <<
		"ADAPT" << name << "\t" <<
		"RD" << name << "\t" <<
		"ACT" << name << "\t" <<
		"SAD" << name << "\t" <<
		"FAD" << name << "\t" <<
		"IA" << name << "\t" <<
		"IAC" << name << "\t" <<
		"AC" << name << "\t" <<
		"SAC" << name << "\t" <<
		"GAC" << name << "\t" <<
		"ADCOSTS" << name << "\t" ;
	t = 0;
}
// writes timestep
void RICEEconAgent::writeStep(std::fstream& output){
	output << traj.pop[ssp][t] << "\t" <<
		traj.tfp[ssp][t] << "\t" <<
		traj.gdpbase[ssp][t] << "\t" <<
		traj.sigma[ssp][t] << "\t" <<
		traj.eind_bau[ssp][t] << "\t" <<
		traj.mx[t] << "\t" <<
		traj.ax[t] << "\t" <<
		traj.bx[t] << "\t" <<
		traj.eland[t] << "\t" <<
		traj.k[t] << "\t" <<
		traj.ygross[t] << "\t" <<
		traj.miu[t] << "\t" <<
		traj.eind[t] << "\t" <<
		traj.e[t] << "\t" <<
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
		traj.periodu[t] << "\t" <<
		traj.cemutotper[t] << "\t" <<
		traj.omega[t] << "\t" <<
		traj.tatm_local[t] << "\t" <<
		traj.damfrac[t] << "\t" <<
		traj.komega[t] << "\t" <<
		traj.basegrowthcap[t] << "\t" <<
		traj.ynet_estimated[t] << "\t" <<
		traj.impact[t] << "\t" <<
		traj.adapt[t] << "\t" <<
		traj.rd[t] << "\t" <<
		traj.act[t] << "\t" <<
		traj.sad[t] << "\t" <<
		traj.fad[t] << "\t" <<
		traj.ia[t] << "\t" <<
		traj.iac[t] << "\t" <<
		traj.ac[t] << "\t" <<
		traj.sac[t] << "\t" <<
		traj.gac[t] << "\t" <<
		traj.adcosts[t] << "\t" ;
	t++;
}
// sets ssp 
void RICEEconAgent::setSsp(int ssps){
	ssp = ssps-1;
	return;
}
// sets damages type
void RICEEconAgent::setDamages(int damages){
	params.damagesType = static_cast<DamagesType>(damages);
	return;
}
// frees allocated memory
void RICEEconAgent::econAgentDelete(){
	for (int idxssp=0; idxssp<5;idxssp++){
		delete[] traj.eind_bau[idxssp];
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
	delete[] traj.miu_up;
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
	delete[] traj.periodu;
	delete[] traj.cemutotper;
	delete[] traj.e;
	delete[] traj.omega;
	delete[] traj.tatm_local;
	delete[] traj.damfrac;
	delete[] traj.komega;
	delete[] traj.basegrowthcap;
	delete[] traj.ynet_estimated;
	delete[] traj.impact;
	delete[] traj.adapt;
	delete[] traj.rd;
	delete[] traj.act;
	delete[] traj.sad;
	delete[] traj.fad;
	delete[] traj.ia;
	delete[] traj.iac;
	delete[] traj.ac;
	delete[] traj.sac;
	delete[] traj.gac;
	delete[] traj.adcosts;
	return;
}