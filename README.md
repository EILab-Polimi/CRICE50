General simulation settings are defined in the folder

	settings:
		generalSettings.txt: horizon, (tstep), carbon_model, climate_model
		globalEconParams.txt: nagents, elasmu, prstp, ineqav, RPCutoffMetric, utility
		EconAgentParams.txt: gama, dk, dela0, deland, ssp, damages, RPCutoffInd, TempLimit, Eland, DecisionMakersType
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

	data_macc:
		mx_multiplier.csv
		macc_coeffs.csv


To run:
1) type "make"
2) type "./RICE50++"

An output file is produced: simulationOutput.txt