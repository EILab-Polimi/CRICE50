# CRICE50: a C++ simulator of RICE50

This repository contains the C++ simulation version of the cost-benefit integrated assessment model RICE50+.

For the original GAMS model see: https://github.com/witch-team/RICE50xmodel

This model version is used to perform cooperative welfare optimization deciding investment in mitigation, adaptation, and climate finance in the 57 regions using a control policy. The policy is optimized using direct policy search using the EMODPS algorithm.

# Contents

The data needed to populate the model is contained in the `data_ed57` folder, divided into categories by subfolders:
- `data_baseline`: baseline socio economic drivers
- `data_climate_regional`: regional climate downscaling
- `data_climate_witch`: data for the WITCH climate model
- `data_coacch`: COACCH damage functions
- `data_economy`: initial capital stock and savings rate
- `data_land_use`: land-use data
- `data_macc`: marginal abatement cost curves
- `regions_mapping`: converting regions to ISO codes

The script used for optimization and postprocessing of obtained policy parametrizations (`runBorgRICE50.sh`) is available inside the `opt` folder.


The `settings` folder contains settings and parameters for the simulation:
- Parameters:
	- Region specific adaptation parameters (original files and their application to 57 regions): `AdaptCoeffOrigADRICE.txt`, `AdaptCoeffOrigADWITCH.txt`, `AdaptCoeffOrigED57.txt`
	- General adaptation parameters: `AdaptParams.txt`
	- Climate and carbon model parameters: `DICEcarbonParams.txt`, `DICEclimateParams.txt`, `FAIRCarbonParams.txt`, `FAIRforcothSSP2.txt`, `GeoffroyClimateParams.txt`
	- Damages parameters: `DamagesCoeffWITCH.txt`, `comega_neg.csv`, 
		`comega_pos.csv`, together with the python script to prepare them `prepare_comega.py`
- Settings:
	- General settings: `generalSettings.txt`
	- Global economic settings: `globalEconParams.txt`
	- Regional economic setttings: `EconAgentParams.txt`
	- Agent policy settings: `settingsAgentPolicy.txt`


The `src` folder contains the source files for the simulator.
- `carbon` contains the carbon component (`Carbon.capp` and corresponding header file)
- `climate` contains the temperature component (`Climate.cpp` and corresponding header file)
- `econ` contains the economic component (global economy `Econ.cpp` and regions/agents `EconAgent.cpp` and corresponding header files)
- `RICE.cpp` and the header file describe the RICE model object functions
- `RICEMain.cpp` is the main file

# Simulate no-policy

To simulate a no-policy scenario:
1) open the terminal in the main folder `/CRICE50`
2) type:
 ```bash
 make
 ```
3) type :
```bash
./RICE50++
```
