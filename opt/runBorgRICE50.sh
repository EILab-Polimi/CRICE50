#!/bin/bash

# user input
PROBLEM=RICE50++
NAMEFILE=optRICE50
EMODPS=/Users/angelocarlino/models/emodps
# EMODPS=/home/acarlino/emodps
# Optimization setting
NSEEDS=10

# static intertemporal optimization
NFE=1000
AGENTS=57
HORIZON=58
DECSVARS=2
NVAR=$((AGENTS*HORIZON*DECSVARS))
UB="1.2,1.0"
LB="0.0,0.0"
NITER=NVAR
((NITER /= DECSVARS))
for S in $(seq 2 $NITER)
do
	UB="$UB,1.2,1.0"
	LB="$LB,0.0,0.0"
done

# emodps
NFE=1000
NVAR=114
NOBJ=8
EPS=0.0000001,0.0000001,4,4,1.0,0.5,10,10
UB="1.0"
LB="0.0"
for S in $(seq 2 $NVAR)
do
	UB="$UB,1.0"
	LB="$LB,0.0"
done


# # optimization
# for SEED in $(seq 1 $NSEEDS)
# do
# OUTFILE=./opt/BorgOutput/${NAMEFILE}_${SEED}.out
# RUNFILE=./opt/BorgOutput/rntdynamics_${SEED}.txt
# cd ..
# ${EMODPS}/borg/borg.exe -n ${NFE} -v ${NVAR} -o ${NOBJ} -s ${SEED} -l ${LB} -u ${UB} -e ${EPS} -f ${OUTFILE} -R ${RUNFILE} -F 1000 ${PROBLEM}
# done
# echo "optimization terminated"

cd ..

MYPATH="$(pwd)/opt/Hydrocalc/BorgOutput/"
FILENAME=optRICE50
START_COLUMN=115
FINISH_COLUMN=122
NUM_SEEDS=$NSEEDS
 
for ((S=1; S<=$NUM_SEEDS; S++))
do
 echo "Processing SEED $S"
 sed '/^\#/ d' ${MYPATH}${FILENAME}_${S}.out > temp.out
 cat temp.out | cut -d ' ' -f ${START_COLUMN}-${FINISH_COLUMN} > ${MYPATH}${FILENAME}_${S}_obj.txt
 rm temp.out
done

# Extract reference set

JAVA_ARGS="-classpath ./src/moeaframework/MOEAFramework-1.17-Executable.jar:."


java ${JAVA_ARGS} org.moeaframework.util.ReferenceSetMerger -e ${EPS} -o ${MYPATH}${FILENAME}.reference ${MYPATH}${FILENAME}*_obj.txt
