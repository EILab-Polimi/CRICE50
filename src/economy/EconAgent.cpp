#include "EconAgent.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include <algorithm>

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
	in.close();
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
	// in.open("./settings/EconAgentParams.txt", ios_base::in);
	// if (!in){
	// 	cout << "The general settings file specified could not be found!" << endl;
	//     exit(1);
	// }
	// while (sJunk!="gama"){
	// 	in >>sJunk;
	// }
	// in >> params.gama;
	// while (sJunk!="dk"){
	// 	in >>sJunk;
	// }
	// in >> params.dk;
	// while (sJunk!="dela0"){
	// 	in >>sJunk;
	// }
	// in >> params.dela0;
	// while (sJunk!="deland"){
	// 	in >>sJunk;
	// }
	// in >> params.deland;
	return;
}
// simulates one time step
void RICEEconAgent::nextStep(){
	e[t] = 20.0 * std::max(0.0,std::min(1.0, 1.0 - ((double)t) / 5.0));
	std::cout << "\t\tHere the region " << name << " evolves to the step " << t+1 << " emitting (GtCO2):" << e[t] << std::endl;
	t++;
	return;
}
// frees allocated memory
void RICEEconAgent::econAgentDelete(){
	for (int ssp=0; ssp<5;ssp++){
		delete[] traj.sigma[ssp];
	}
	delete[] traj.sigma;
	delete[] e;
	return;
}