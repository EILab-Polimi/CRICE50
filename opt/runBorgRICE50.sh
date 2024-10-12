#!/bin/bash

# Post-processing or optimization
postprocessing=true
optimization=false

# Name of the executable
PROBLEM=RICE50++

# Name of the output file
NAMEFILE=optRICE50

# EMODPS folder
EMODPS=/home/acarlino/emodps
# EMODPS=/Users/angelocarlino/models/emodps

# Optimization settings
NSEEDS=5
NFE=500000
FRUN=10000
NVAR=175
NOBJ=1
EPS=1e-8
UB="2.0"
LB="-2.0"
for S in $(seq 2 $NVAR)
do
	UB="$UB,2.0"
	LB="$LB,-2.0"
done

# Optimization

if [ "$optimization" = true]; then
	for SEED in $(seq 1 $NSEEDS)
	do
		OUTFILE=./opt/BorgOutput/${NAMEFILE}_${SEED}.out
		RUNFILE=./opt/BorgOutput/rntdynamics_${SEED}.txt
		${EMODPS}/borg/borg.exe -n ${NFE} -v ${NVAR} -o ${NOBJ} -s ${SEED} -l ${LB} -u ${UB} -e ${EPS} -f ${OUTFILE} -R ${RUNFILE} -F ${FRUN} ${PROBLEM} &
	done
	echo "optimization terminated"
fi


# Post-processing of the optimization output

if [ "$postprocessing" = true ]; then
	MYPATH="$(pwd)/BorgOutput/"
	FILENAME=optRICE50
	START_COLUMN=$((${NVAR}+1))
	FINISH_COLUMN=$((${NVAR}+${NOBJ}))
	NUM_SEEDS=$NSEEDS
	
	for ((S=1; S<=$NUM_SEEDS; S++))
	do
		echo "Processing SEED $S"
		sed '/^\#/ d' ${MYPATH}${FILENAME}_${S}.out > temp.out
		cat temp.out | cut -d ' ' -f ${START_COLUMN}-${FINISH_COLUMN} > ${MYPATH}${FILENAME}_${S}_obj.txt
		rm temp.out
	done

	# Extract reference set
	JAVA_ARGS="-classpath ../src/moeaframework/MOEAFramework-1.17-Executable.jar:."
	java ${JAVA_ARGS} org.moeaframework.util.ReferenceSetMerger -e ${EPS} -o ${MYPATH}${FILENAME}.reference ${MYPATH}${FILENAME}*_obj.txt

fi

