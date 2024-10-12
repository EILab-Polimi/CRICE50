# CRICE50: a C++ simulator of RICE50

This repository contains the C++ simulation version of the cost-benefit integrated assessment model RICE50+.

For the original GAMS model see: https://github.com/witch-team/RICE50xmodel

This model version is used to perform cooperative welfare optimization deciding investment in mitigation, adaptation, and climate finance in the 57 regions using a control policy. The policy is optimized using direct policy search using the EMODPS algorithm.

# Contents

The data needed to populate the model are contained in the 'data_ed57' folder.

The 'opt' folder contains the script used for optimization and postprocessing of solutions obtained.

The 'settings' folder contains settings and parameters for the simulation.

The 'src' folder contains the source files for the simulator.

# Simulate no-policy

To simulate a no-policy scenario:
1) type 
	make
2) type 
	./RICE50++

Simulation and optimization settings are defined in the folder

	settings:
		generalSettings.txt: horizon, (tstep), carbon_model, climate_model
		globalEconParams.txt: nagents, elasmu, prstp, ineqav, RPCutoffMetric, utility
		EconAgentParams.txt: gama, dk, dela0, deland, ssp, damages, RPCutoffInd, TempLimit, Eland, DecisionMakersType, max_miu
settings for DICE carbon and climate models are in the folder
	
	settings:
		DICEcarbonParams.txt: DICE carbon module parameters from DICE2016
		DICEclimateParams.txt: DICE climate module parameters from DICE2016
Parameters for WITCH carbon and climate models are in the folder

	data_climate_witch:
		tempc.csv
		cmphi.csv
		rfc.csv
		wcum_emi0.csv
		oghg_coeff.csv

specific settings and variables for the economic agents are read from different folders:

	settings:
		globalEconParams.txt
		EconAgentParams.txt

	data_baseline:
		ssp_cintensity.csv
		ssp_pop.csv
		ssp_tfp.csv
		ssp_ykali.csv

	data_climate_regional:
		climate_region_coef.csv

	data_economy:
		k0.csv
		s0.csv

	data_land_use:
		etree_bau.csv
		etree_opt.csv

	data_macc:
		mx_multiplier.csv
		macc_coeffs.csv


To run:
1) type "make"
2) type "./RICE50++"

An output file is produced: simulationOutput.txt