#!/bin/bash

# user input
PROBLEM=RICE50++
NAMEFILE=optRICE50
EMODPS=/Users/angelocarlino/models/emodps
# Optimization setting
NSEEDS=1
NFE=1000
AGENTS=57
HORIZON=58
DECSVARS=2
NVAR=$((AGENTS*HORIZON*DECSVARS))
echo $NVAR

NOBJ=1
EPS=1

# static intertemporal optimization
UB="1.2,1.0"
LB="0.0,0.0"

NITER=NVAR
((NITER /= DECSVARS))

for S in $(seq 2 $NITER)
do
	UB="$UB,1.2,1.0"
	LB="$LB,0.0,0.0"
done


# optimization
for SEED in $(seq 1 $NSEEDS)
do
OUTFILE=./opt/BorgOutput/${NAMEFILE}_${SEED}.out
RUNFILE=./opt/BorgOutput/rntdynamics_${SEED}.txt
PARAM=opt
cd ..
${EMODPS}/borg/borg.exe -n ${NFE} -v ${NVAR} -o ${NOBJ} -s ${SEED} -l ${LB} -u ${UB} -e ${EPS} -f ${OUTFILE} -R ${RUNFILE} -F 100 ${PROBLEM} ${PARAM}
done
echo "optimization terminated"

# MYPATH="$(pwd)/BorgOutput/"
# FILENAME=optADCDICE2016
# START_COLUMN=201
# FINISH_COLUMN=201
# NUM_SEEDS=$NSEEDS

# for ((S=1; S<=$NUM_SEEDS; S++))
# do
#  echo "Processing SEED $S"
#  sed '/^\#/ d' ${MYPATH}${FILENAME}_${S}.out > temp.out
#  cat temp.out | cut -d ' ' -f ${START_COLUMN}-${FINISH_COLUMN} > ${MYPATH}${FILENAME}_${S}_obj.txt
#  rm temp.out
# done

# # Extract reference set

# JAVA_ARGS="-classpath ./src/MOEAFramework-1.17-Executable.jar:."


# java ${JAVA_ARGS} org.moeaframework.util.ReferenceSetMerger -e ${EPS} -o ${MYPATH}${FILENAME}.reference ${MYPATH}${FILENAME}*_obj.txt
