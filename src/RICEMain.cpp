/*        _            _________         
    _____(_)_______  / ____/ __ \   __     __         
   / ___/ / ___/ _ \/___ \/ / / / _/ /_  _/ /_         
  / /  / / /__/  __/___/ / /_/ //_ x__//_ x__/         
 /_/  /_/\___/\___/_____/\____/  /_/    /_/         

TODO: Here we will put authors and license
*/
#include "RICE.h"
#include "./moeaframework/moeaframework.h"
#include <iostream>
#include <time.h>
#include <string>
#include <math.h>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include <numeric>

// Reads a list of files in a directory
void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory)
{
#ifdef WINDOWS
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return; /* No files found */

    do {
        const std::string file_name = file_data.cFileName;
        const std::string full_file_name = directory + "/" + file_name;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (file_name[0] == '.')
            continue;

        if (is_directory)
            continue;

        out.push_back(full_file_name);
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
#else
    DIR *dir;
    struct dirent *ent;
    class stat st;

    dir = opendir(directory.c_str());
    while ((ent = readdir(dir)) != NULL) {
        const std::string file_name = ent->d_name;
        const std::string full_file_name = directory + "/" + file_name;

        if (file_name[0] == '.')
            continue;

        if (stat(full_file_name.c_str(), &st) == -1)
            continue;

        const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        if (is_directory)
            continue;

        out.push_back(full_file_name);
    }
    closedir(dir);
#endif
}

int main(int argc, char* argv[])
{	
	clock_t start, end;
	start = clock();

	// ==== MODEL SETTINGS ==========
	// here we should be reading the input file
	// and fix the settings for the simulations 
	// to be run

	// do we want to consider different delta t ?
	// int delta_t = 1;

	// ==== MODEL BUILD ==========	
	// here we create the RICE instance
	// and the associated pointer

    // std::cout << "Loading input data: " << std::endl;
	RICE rice;
    // std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
    // std::cout << "Simulating: " << std::endl;
	RICE* riceptr = &rice;

	int nobjs = riceptr->getNObjs();
	int nvars = riceptr->getNVars();	
	double objs[nobjs];
	double vars[nvars];
	// std::cout << nvars << std::endl;

	if (riceptr->robustness == 1){
		double ECSvalues[3] = {2.5, 3.0, 4.0};
		// get list of solution files to be simulated 
		std::vector<std::string> listFiles;
		listFiles.reserve(484);
		std::string solDir = "./RICE50++_Inputs";
		GetFilesInDirectory(listFiles, solDir);
		// file streams:  input & output  
		std::fstream solFile;
		std::fstream robustnessOutput;
		// write header
		robustnessOutput.open("./robustnessOutput.txt", std::ios_base::out);
		robustnessOutput << "Solution\tECS\tSSP\tDamages\t" << 
			"Welfare\tT(2100)\tTmax\tGini(2100)\t" <<
			"90thGDPpc(2100)\t80thGDPpc(2100)\t" << 
			"20thGDPpc(2100)\t10thGDPpc(2100)\t" << std::endl;

		for (int nfiles = 0; nfiles < listFiles.size() ; nfiles++){
			// get file name
			std::string nameSol = listFiles[nfiles];
			nameSol.erase( nameSol.begin(), nameSol.begin() + 18 );
			nameSol.erase( nameSol.end() - 4, nameSol.end() );
			// open solution file to be simulated
			solFile.open(listFiles[nfiles], std::ios_base::in);
			if (!solFile) {
				std::cerr << "Error: file could not be opened" << std::endl;
	    		exit(1);
	    	}
	    	// read file into variables
			for (int varidx = 0; varidx < nvars ; varidx++){
				solFile >> vars[varidx];
			}
			solFile.close();
			// set the variables read above
			riceptr->setVariables(vars);
			for (int idxECS = 0; idxECS<3; idxECS++){
				riceptr->setECS(ECSvalues[idxECS]);
				for (int ssp = 1; ssp <= 5; ssp++){
					// set ssp
					riceptr->setSsp(ssp);
					for (int damages = BURKESR; damages < DAMAGEERR; damages++){
						// set damages
						riceptr->setDamages(damages);
						// simulate
						riceptr->simulate();
						// report
						riceptr->reportObjs(nameSol, ECSvalues[idxECS], ssp, damages, robustnessOutput);
					}
				}				
			}
		}
		robustnessOutput.close();			
	}
	else if (riceptr->robustness == 2){
		// double ECSvalues[3] = {2.5, 3.0, 4.0};
		// get list of solution files to be simulated 
		std::vector<std::string> listFiles;
		listFiles.reserve(484);
		std::string solDir = "./InputSim";
		GetFilesInDirectory(listFiles, solDir);
		// file streams:  input & output  
		std::fstream solFile;

		for (int nfiles = 0; nfiles < listFiles.size() ; nfiles++){
			// get file name
			std::string nameSol = listFiles[nfiles];
			// std::cout << nameSol << std::endl;
			nameSol.erase( nameSol.begin(), nameSol.begin() + 11 );
			nameSol.erase( nameSol.end() - 4, nameSol.end() );
			// std::cout << nameSol << std::endl;
			// open solution file to be simulated
			solFile.open(listFiles[nfiles], std::ios_base::in);
			if (!solFile) {
				std::cerr << "Error: file " << listFiles[nfiles] << " could not be opened" << std::endl;
	    		exit(1);
	    	}
	    	// read file into variables
			for (int varidx = 0; varidx < nvars ; varidx++){
				solFile >> vars[varidx];
			}
			solFile.close();
			// set the variables read above
			riceptr->setVariables(vars);
			// for (int idxECS = 0; idxECS<3; idxECS++){
			// 	riceptr->setECS(ECSvalues[idxECS]);
				for (int ssp = 1; ssp <= 5; ssp++){
					// set ssp
					riceptr->setSsp(ssp);
					for (int damages = BURKESR; damages < DAMAGEERR; damages++){
						// set damages
						riceptr->setDamages(damages);
						// simulate
						riceptr->simulate();
						// write output
						std::string filename = "./solsOutput/";
						filename.append(nameSol);
						// filename.append("_").append(std::to_string(ECSvalues[idxECS]));
						filename.append("_SSP").append(std::to_string(ssp));
						filename.append("_DAMAGE").append(std::to_string(damages)).append(".txt");
						riceptr->writeSimulation(filename);
					}
				}				
			// }
		}
	}
	else if (riceptr->robustness==3){
	//validation
		std::vector<std::string> listFiles;
		std::string solDir = "./InputVal";
		GetFilesInDirectory(listFiles, solDir);
		// file streams:  input & output  
		std::fstream solFile;

		for (int nfiles = 0; nfiles < listFiles.size() ; nfiles++){
			// get file name
			std::string nameSol = listFiles[nfiles];
			nameSol.erase( nameSol.begin(), nameSol.begin() + 11 );
			nameSol.erase( nameSol.end() - 4, nameSol.end() );
			std::cout << nameSol << std::endl;
			// open solution file to be simulated
			solFile.open(listFiles[nfiles], std::ios_base::in);
			if (!solFile) {
				std::cerr << "Error: file " << listFiles[nfiles] << " could not be opened" << std::endl;
	    		exit(1);
	    	}
	    	// read file into variables
			for (int varidx = 0; varidx < nvars ; varidx++){
				solFile >> vars[varidx];
			}
			solFile.close();
			// set the variables read above
			riceptr->setVariables(vars);
			riceptr->simulateUnc(objs);
			std::fstream objsFile;
			objsFile.open(std::string("./ObjsVal/").append(nameSol).append(".txt"), std::ios_base::out);
			for (int obj=0; obj < nobjs; obj++){
				objsFile << objs[obj] << "\t";
			}
			objsFile.close();

			// for (int idxECS = 0; idxECS<3; idxECS++){
			// std::vector<double> allobjs;
			// std::vector<double> welfare;
			// std::vector<double> y15c;
			// std::vector<double> ineq;
			// std::vector<double> net;
			// int ssp = 2;
			// int damages = KALKUHL;
			// double adapteff = 1.0;
			// riceptr->setSsp(ssp);
			// riceptr->setDamages(damages);
			// riceptr->setAdaptEff(adapteff);
			// double y15C = 0.0;
			// riceptr->simulate();
			// welfare.push_back(-riceptr->econ->utility);
			// for (int tidx = 0; tidx < riceptr->horizon; tidx++){
			// 	if (riceptr->climate->tatm[tidx] > 1.5){
			// 		y15C += riceptr->climate->tatm[tidx] - 1.5;
			// 	}
			// }
			// y15c.push_back(y15C);
			// ineq.push_back(riceptr->econ->computePrctiles7525());
			std::string filename = "./valOutput/";
			filename.append(nameSol);
			filename.append("_SSP2")
				.append("_DAMAGE8")
				.append("_ADAPTEFF1")
				.append("_ECS3.0")
				.append("_TCR1.8")
				.append(".txt");
			riceptr->writeSimulation(filename);

			// double adapteff_v[2] = {0.0, 1.0}; 
			// srand(20000);			
			// for (int ssp = 1; ssp <= 5; ssp++){
			// 	// set ssp
			// 	riceptr->setSsp(ssp);
			// 	for (int damages = BURKESR; damages < DAMAGEERR; damages++){
			// 		// set damages
			// 		riceptr->setDamages(damages);
			// 		for (int adEff=0; adEff < (sizeof(adapteff_v)/sizeof(*adapteff_v)); adEff++){
			// 			riceptr->setAdaptEff(adapteff_v[adEff]);
			// 			for (int nsim=0; nsim<5; nsim++){
			// 				riceptr->climate->sampleUnc();
			// 				double y15C = 0.0;
			// 				riceptr->simulate();
			// 				welfare.push_back(-riceptr->econ->utility);
			// 				for (int tidx = 0; tidx < riceptr->horizon; tidx++){
			// 					if (riceptr->climate->tatm[tidx] > 1.5){
			// 						y15C += riceptr->climate->tatm[tidx] - 1.5;
			// 					}
			// 				}
			// 				y15c.push_back(y15C);
			// 				ineq.push_back(riceptr->econ->computePrctiles7525());
			// 				net.push_back(riceptr->econ->computeNET());	
			// 				std::string filename = "./valOutput/";
			// 				filename.append(nameSol);
			// 				// filename.append("_").append(std::to_string(ECSvalues[idxECS]));
			// 				filename.append("_SSP").append(std::to_string(ssp))
			// 					.append("_DAMAGE").append(std::to_string(damages))
			// 					.append("_ADAPTEFF").append(std::to_string(adapteff_v[adEff]).substr(0,4))
			// 					.append("_ECS").append(std::to_string(riceptr->climate->getECS()).substr(0,4))
			// 					.append("_TCR").append(std::to_string(riceptr->climate->getTCR()).substr(0,4))
			// 					.append(".txt");
			// 				riceptr->writeSimulation(filename);
			// 			}
			// 		}
			// 	}
			// }
			// double sum = std::accumulate(std::begin(welfare), std::end(welfare), 0.0);
			// allobjs.push_back(sum / welfare.size());
			// allobjs.push_back(*std::max_element(welfare.begin(), welfare.end()));
			// sum = std::accumulate(std::begin(y15c), std::end(y15c), 0.0);
			// allobjs.push_back(sum / y15c.size());
			// allobjs.push_back(*std::max_element(y15c.begin(), y15c.end()));
			// sum = std::accumulate(std::begin(ineq), std::end(ineq), 0.0);
			// allobjs.push_back(sum / ineq.size());
			// allobjs.push_back(*std::max_element(ineq.begin(), ineq.end()));
			// for (int obj = 0; obj < nobjs; obj++){
			// 	objs[obj] = allobjs[obj];
			// }
			// std::fstream objsFile;
			// objsFile.open(std::string("./ObjsVal/").append(nameSol).append(".txt"), std::ios_base::out);
			// for (int obj=0; obj < nobjs; obj++){
			// 	objsFile << objs[obj] << "\t";
			// }
			// objsFile.close();
		}
	}
	else{
		// ==== SIMULATION EXECUTION ==========
		if (riceptr->econ->params.DMType == BAU){
			riceptr->simulate();				
			std::cout << riceptr->econ->utility << std::endl;	
		}
		// else{
		// 	MOEA_Init(nobjs, 0);
		// 	while (MOEA_Next_solution() == MOEA_SUCCESS) {
		// 		MOEA_Read_doubles(nvars, vars);
		// 		riceptr->setVariables(vars);
		// 		riceptr->simulate();
		// 		objs[0] =  - riceptr->econ->utility;
		// 		MOEA_Write(objs, NULL);
		// 	}
		// }
		else{
			MOEA_Init(nobjs, 0);
			while (MOEA_Next_solution() == MOEA_SUCCESS) {
				MOEA_Read_doubles(nvars, vars);
				riceptr->setVariables(vars);
				riceptr->simulateUnc(objs);
				// riceptr->simulate();
				// objs[0] =  - riceptr->econ->utility;
				MOEA_Write(objs, NULL);
			}
		}
		if (riceptr->writefile == 1){
			riceptr->writeSimulation("output.txt");
		}
	}
	// ==== POST PROCESSING ==========
    // std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
    // std::cout << "Freeing memory: " << std::endl;
	// delete memory used by RICE
	riceptr->RICE_delete();

	// end
    end = clock();
    std::cout << "total time elapsed: " << ((end - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
	return 0;
}