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

	// ==== SIMULATION EXECUTION ==========
	if (riceptr->econ->params.DMType == BAU){
		riceptr->simulate();				
		std::cout << riceptr->econ->utility << std::endl;	
	}
	else{
		MOEA_Init(nobjs, 0);
		while (MOEA_Next_solution() == MOEA_SUCCESS) {
			MOEA_Read_doubles(nvars, vars);
			riceptr->setVariables(vars);
			riceptr->simulate();
			objs[0] =  - riceptr->econ->utility;
			MOEA_Write(objs, NULL);
		}
	}


	// DRAFT CODE FOR MULTIPLE ITERATIONS
	// for (int iter=0; iter<10; iter++){
	//    std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
	//    std::cout << "Simulating: " << std::endl;

	//    std::cout << "time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;

	//    riceptr->resetTidx();
		// riceptr->simulate();
	// }

    // std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
    // std::cout << "Writing output data: " << std::endl;

	// ==== POST PROCESSING ==========

	// export simulation results in output file

	riceptr->writeSimulation();
    // std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
    // std::cout << "Freeing memory: " << std::endl;
	// delete memory used by RICE 
	riceptr->RICE_delete();

	// end
    end = clock();
    std::cout << "total time elapsed: " << ((end - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
	return 0;
}