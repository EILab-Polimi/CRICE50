#include "EconAgent.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<algorithm>
#include<math.h>
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
	in.close();
	// THIS HAS TO BE FIXED
	params.prstp = 0.015;
	params.elasmu = 1.45;
	std::string line;
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
	params.damage_type = 0;
	params.beta_bhm = 0.0127184;
	params.beta_bhm2 = -0.0004871;	
	params.beta_djo = 0.0261;
	// params.beta_djo = 0.0261 - 0.01655;
	params.beta_k = -0.0586;
	return;
}
void RICEEconAgent::readBaseline(int hrzn){
	std::fstream in;
	std::string line;
	//carbon intensity
	traj.sigma = new double * [5];
	for (int ssp=0; ssp<5; ssp++){
		traj.sigma[ssp] = new double[hrzn];
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
	for (int ssp=0; ssp<5; ssp++){
		traj.pop[ssp] = new double[hrzn];
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
	for (int ssp=0; ssp<5; ssp++){
		traj.tfp[ssp] = new double[hrzn];
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
	for (int ssp=0; ssp<5; ssp++){
		traj.gdpbase[ssp] = new double[hrzn];
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
	traj.gdp = new double[hrzn];
	traj.eind = new double[hrzn];
	traj.k = new double[hrzn];
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
	traj.miu = new double[hrzn];
	traj.s = new double[hrzn];
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
	traj.mx = new double[hrzn];
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
	traj.ax = new double[hrzn];
	traj.bx = new double[hrzn];
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
	traj.eland = new double[hrzn];
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
	traj.abatecost = new double[hrzn];
	traj.i = new double[hrzn];
	traj.ygross = new double[hrzn];
	traj.ynet = new double[hrzn];
	traj.y = new double[hrzn];
	traj.damages = new double[hrzn];
	traj.c = new double[hrzn];
	traj.cpc = new double[hrzn];
	traj.ri = new double[hrzn];
	traj.cprice = new double[hrzn];
	traj.omega = new double[hrzn];
	traj.omega[0] = 0.0;
	return;
}
// simulates one time step
void RICEEconAgent::nextStep(double* tatm){
	nextAction();
	// compute ygross
	traj.ygross[t] = traj.tfp[ssp-1][t] * 
		pow(traj.k[t], params.gama) * 
		pow(traj.pop[ssp-1][t]/1000.0, 1 - params.gama);
	// compute emissions
	traj.eind[t] = traj.sigma[ssp-1][t] * 
		traj.ygross[t] * (1 - traj.miu[t]);
	e[t] = traj.eind[t] + traj.eland[t];
	computeDamages(tatm);	
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
	traj.cpc[t] = 1000 * traj.c[t] / traj.pop[ssp-1][t];
	// capital stock step transition
	traj.k[t+1] = traj.k[t] * pow(1-params.dk, 5) + 5 * traj.i[t];
	if (t >= 1){
		traj.ri[t-1] = (1 + params.prstp) * 
			pow(traj.cpc[t]/traj.cpc[t-1], params.elasmu/5) - 1;
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
void RICEEconAgent::computeDamages(double* tatm){
	double tatm_local = params.alpha_tatm + params.beta_tatm * tatm[t];
	if (params.damage_type == 0){
		traj.damages[t] = 0.0;		
	}
	else if (params.damage_type==1){
		//BURKE - 4 types (2 with rich_poor cutoff)
		double bimpact = params.beta_bhm* tatm_local + 
			params.beta_bhm2* pow(tatm_local,2)
			- params.beta_bhm* params.base_tatm 
			- params.beta_bhm2* pow(params.base_tatm,2);
		double komega = pow((traj.k[t] * pow(1 - params.dk , 5) +
			5 * traj.s[t] * traj.tfp[ssp-1][t] * pow(traj.k[t], params.gama) *
			pow(traj.pop[ssp-1][t]/1000.0, 1.0 - params.gama) * (1.0 / 1.0 + traj.omega[t]))
			/ traj.k[t], params.gama);
		if (t < horizon - 1){
			double basegrowthcap = pow((traj.gdpbase[ssp-1][t+1]/traj.pop[ssp-1][t+1])
				/ (traj.gdpbase[ssp-1][t]/traj.pop[ssp-1][t]), 1.0/5.0) - 1;
			traj.omega[t+1] = (1 + traj.omega[t]) * traj.tfp[ssp-1][t+1]/traj.tfp[ssp-1][t]
				* pow(traj.pop[ssp-1][t+1]/traj.pop[ssp-1][t], 1 - params.gama)
				* traj.pop[ssp-1][t]/traj.pop[ssp-1][t+1] * komega
				/ pow(1 + basegrowthcap + bimpact, 5) - 1;
		}
		double damfrac = 1 - (1 / ( 1 + traj.omega[t]));
		double ynet_estimated = std::min(std::max(traj.ygross[t] 
			* (1 - damfrac), pow(10.0, -4.0) * traj.gdpbase[ssp-1][t]), 
			2 * traj.gdpbase[ssp-1][t]);
		traj.damages[t] = traj.ygross[t] - ynet_estimated;
	}
	else if (params.damage_type==2){
		//DJO with rich_poor cutoff
		double djoimpact = params.beta_djo * 
			(tatm_local - params.base_tatm); 
		double komega = pow((traj.k[t] * pow(1 - params.dk , 5) +
			5 * traj.s[t] * traj.tfp[ssp-1][t] * pow(traj.k[t], params.gama) *
			pow(traj.pop[ssp-1][t]/1000.0, 1.0 - params.gama) * (1.0 / 1.0 + traj.omega[t]))
			/ traj.k[t], params.gama);
		if (t < horizon - 1){
			double basegrowthcap = pow((traj.gdpbase[ssp-1][t+1]/traj.pop[ssp-1][t+1])
				/ (traj.gdpbase[ssp-1][t]/traj.pop[ssp-1][t]), 1.0/5.0) - 1;
			traj.omega[t+1] = (1 + traj.omega[t]) * traj.tfp[ssp-1][t+1]/traj.tfp[ssp-1][t]
				* pow(traj.pop[ssp-1][t+1]/traj.pop[ssp-1][t], 1 - params.gama)
				* traj.pop[ssp-1][t]/traj.pop[ssp-1][t+1] * komega
				/ pow(1 + basegrowthcap + djoimpact, 5) - 1;
		}
		double damfrac = 1 - (1 / ( 1 + traj.omega[t]));
		double ynet_estimated = std::min(std::max(traj.ygross[t] 
			* (1 - damfrac), pow(10.0, -4.0) * traj.gdpbase[ssp-1][t]), 
			2 * traj.gdpbase[ssp-1][t]);
		traj.damages[t] = traj.ygross[t] - ynet_estimated;
	}
	else if (params.damage_type==3){
		//KAHN
		double tatm_mavg = 0.0;
		for (int tidx=1; tidx<7; tidx++){
			if (t-tidx < 6){
				tatm_mavg += params.base_tatm;				
			}
			else{
				tatm_mavg += tatm[t-tidx];								
			}
		}
		tatm_mavg = tatm_mavg/6;
		double kimpact = params.beta_k * 
			(tatm_local - tatm_mavg); 
		double komega = pow((traj.k[t] * pow(1 - params.dk , 5) +
			5 * traj.s[t] * traj.tfp[ssp-1][t] * pow(traj.k[t], params.gama) *
			pow(traj.pop[ssp-1][t]/1000.0, 1.0 - params.gama) * (1.0 / 1.0 + traj.omega[t]))
			/ traj.k[t], params.gama);
		if (t < horizon - 1){
			double basegrowthcap = pow((traj.gdpbase[ssp-1][t+1]/traj.pop[ssp-1][t+1])
				/ (traj.gdpbase[ssp-1][t]/traj.pop[ssp-1][t]), 1.0/5.0) - 1;
			traj.omega[t+1] = (1 + traj.omega[t]) * traj.tfp[ssp-1][t+1]/traj.tfp[ssp-1][t]
				* pow(traj.pop[ssp-1][t+1]/traj.pop[ssp-1][t], 1 - params.gama)
				* traj.pop[ssp-1][t]/traj.pop[ssp-1][t+1] * komega
				/ pow(1 + basegrowthcap + kimpact, 5) - 1;
		}
		double damfrac = 1 - (1 / ( 1 + traj.omega[t]));
		double ynet_estimated = std::min(std::max(traj.ygross[t] 
			* (1 - damfrac), pow(10.0, -4.0) * traj.gdpbase[ssp-1][t]), 
			2 * traj.gdpbase[ssp-1][t]);
		traj.damages[t] = traj.ygross[t] - ynet_estimated;	
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
		"OMEGA" << name << "\t";
	t = 0;
}
// writes timestep
void RICEEconAgent::writeStep(std::fstream& output){
	output << traj.pop[ssp-1][t] << "\t" <<
		traj.tfp[ssp-1][t] << "\t" <<
		traj.gdpbase[ssp-1][t] << "\t" <<
		traj.sigma[ssp-1][t] << "\t" <<
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
		traj.omega[t] << "\t";
	t++;
}
// frees allocated memory
void RICEEconAgent::econAgentDelete(){
	for (int ssp=0; ssp<5;ssp++){
		delete[] traj.pop[ssp];
		delete[] traj.tfp[ssp];
		delete[] traj.gdpbase[ssp];
		delete[] traj.sigma[ssp];
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
	return;
}