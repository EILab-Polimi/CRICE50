In the settings folder the simulation settings are defined:
		generalSettings.txt: horizon, (tstep), carbon_model, climate_model
		globalEconParams.txt: nagents, elasmu, prstp, ineqav, RPCutoffMetric
		EconAgentParams.txt: gama, dk, dela0, deland, ssp
specific settings for carbon and climate models:
		DICEcarbonParams.txt: DICE carbon module parameters from DICE2016
		DICEclimateParams.txt: DICE climate module parameters from DICE2016
	Parameters for WITCH carbon and climate models are in the data_climate_witch folder:
		tempc.csv
		cmphi.csv
		rfc.csv
		wcum_emi0.csv
		oghg_coeff.csv
specific settings for the economic agents are read from different folders:
	data_baseline:

	data_climate_regional:

	data_economy:

	data_land_use:

	data_macc:



How to run
(MACOS and Linux):
1) type "make"
2) type "./RICE50++"
