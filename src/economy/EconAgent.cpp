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
	params.prstp = 0.015;
	params.elasmu = 1.45;
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
	in.open("./data/data_baseline/ssp_cintensity.csv");
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
	in.open("./data/data_baseline/ssp_pop.csv");
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
	in.open("./data/data_baseline/ssp_tfp.csv");
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
	in.open("./data/data_baseline/ssp_ykali.csv");
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
	in.open("./data/data_economy/k0.csv");
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
	//macc multiplier
	traj.mx = new double[hrzn];
	in.open("./data/data_macc/mx_multiplier.csv");
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
	in.open("./data/data_macc/macc_coeffs.csv");
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
	in.open("./data/data_land_use/etree_bau.csv");
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

	traj.miu = new double[hrzn];
	traj.s = new double[hrzn];
	traj.i = new double[hrzn];
	traj.ygross = new double[hrzn];
	traj.ynet = new double[hrzn];
	traj.y = new double[hrzn];
	traj.damages = new double[hrzn];
	traj.c = new double[hrzn];
	traj.cpc = new double[hrzn];
	traj.ri = new double[hrzn];
	traj.cprice = new double[hrzn];
	return;
}
// simulates one time step
void RICEEconAgent::nextStep(){
	nextAction();
	// ygross
	traj.ygross[t] = traj.tfp[ssp-1][t] * 
		pow(traj.k[t], params.gama) * 
		pow(traj.pop[ssp-1][t]/1000.0, 1 - params.gama);
	// compute emissions
	traj.eind[t] = traj.sigma[ssp-1][t] * 
		traj.ygross[t] * (1 - traj.miu[t]);
	e[t] = traj.eind[t] + traj.eland[t];
	computeDamages();	
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
	// capital stock transition
	traj.k[t+1] = traj.k[t] * pow(1-params.dk, 5) + 5 * traj.i[t];
	if (t >= 1){
		traj.ri[t-1] = (1 + params.prstp) * 
			pow(traj.cpc[t]/traj.cpc[t-1], params.elasmu/5) - 1;
	}

	std::cout << "\t\tHere the region " << name << " evolves to the step " << t+1 << " emitting (GtCO2):" << e[t] << std::endl;
	t++;
	return;
}
// take next action
void RICEEconAgent::nextAction(){
	// set decision variables
	traj.miu[t] = std::min(1.0, 0.1 * (double) t);
	traj.s[t] = 0.24;
	return;	
}
void RICEEconAgent::computeDamages(){
	traj.damages[t] = 0.0;
	return;
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
	return;
}