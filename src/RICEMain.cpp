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

	int nobjs = riceptr->getNObjs();
	int nvars = riceptr->getNVars();	
	double objs[nobjs];
	double vars[nvars];

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
	else{
		// ==== BAU SIMULATION EXECUTION ==========
		if (riceptr->econ->params.DMType == BAU){
			riceptr->simulate();				
			std::cout << riceptr->econ->utility << std::endl;	
		}
		// ==== SIMULATE BASED ON INPUT  ==========
		else{
			MOEA_Init(nobjs, 0);
			while (MOEA_Next_solution() == MOEA_SUCCESS) {
				MOEA_Read_doubles(nvars, vars);
				riceptr->setVariables(vars);
				riceptr->simulateUnc(objs);
				MOEA_Write(objs, NULL);
			}
		}
		else{
			MOEA_Init(nobjs, 0);
			while (MOEA_Next_solution() == MOEA_SUCCESS) {
				MOEA_Read_doubles(nvars, vars);
				riceptr->setVariables(vars);
				// riceptr->simulateUnc(objs);
				riceptr->simulate();
				objs[0] =  - riceptr->econ->utility;
				MOEA_Write(objs, NULL);
			}
		}
		riceptr->writeSimulation();
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