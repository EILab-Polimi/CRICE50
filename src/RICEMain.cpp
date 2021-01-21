/*        _            _________         
    _____(_)_______  / ____/ __ \   __     __         
   / ___/ / ___/ _ \/___ \/ / / / _/ /_  _/ /_         
  / /  / / /__/  __/___/ / /_/ //_ x__//_ x__/         
 /_/  /_/\___/\___/_____/\____/  /_/    /_/         

TODO: Here we will put authors and license
*/
#include "RICE.h"
#include <iostream>
#include <time.h>

int main(int argc, char* argv[])
{	
	clock_t start, end;
	start = clock();


	// ==== MODEL SETTINGS ==========
	// here we should be reading the input file
	// and fix the settings for the simulations 
	// to be run

	// int carbontype = 1;
	// if (argc > 1){
	// 	carbontype = atoi(argv[1]);		
	// }
	// int horizon = 57; // 57 timesteps mean from 2015 (0) to 2305 (57)

	// do we want to consider different delta t ?
	// int delta_t = 1;

	// ==== MODEL BUILD ==========	
	// here we create the RICE instance
	// and the associated pointer

    std::cout << "Loading input data: " << std::endl;
	RICE rice;
    std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
    std::cout << "Simulating: " << std::endl;

	RICE* riceptr = &rice;

	// ==== SIMULATION EXECUTION ==========

	riceptr->simulate();

	// for (int iter=0; iter<10; iter++){
	 //    std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
	 //    std::cout << "Simulating: " << std::endl;

	 //    std::cout << "time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;

	 //    riceptr->resetTidx();
		// riceptr->simulate();
	// }
    std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
    std::cout << "Writing output data: " << std::endl;

	// ==== POST PROCESSING ==========

	// export simulation results in output file

	riceptr->writeSimulation();
    std::cout << "total time elapsed: " << ((clock() - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
    std::cout << "Freeing memory: " << std::endl;
	// delete memory used by RICE 
	riceptr->RICE_delete();

	// end
    end = clock();
    std::cout << "total time elapsed: " << ((end - start)/double(CLOCKS_PER_SEC)) << " seconds" << std::endl;
	return 0;
}