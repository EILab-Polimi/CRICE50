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
		std::cout << "The general settings file specified could not be found!" << std::endl;
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
	traj.sigmabase = new double * [5]; //5 SSP
	for (int ssp=0; ssp<5; ssp++){
		traj.sigmabase[ssp] = new double[hrzn];
	}
	in.open("./data/data_baseline/ssp_cintensity_adj.csv");
	if (!in){
		std::cout << "The general settings file specified could not be found!" << std::endl;
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
			traj.sigmabase[atoi(idxsspp)-1][stoi(splitline[1])-1] = stod(splitline[3]);
		}
	}
	in.close();
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
		delete[] traj.sigmabase[ssp];
	}
	delete[] traj.sigmabase;
	delete[] e;
	return;
}