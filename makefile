CC = gcc
CXX = g++
ARGS = -O3 -Wall
CXXFLAGS = -c -O3 -Wall -std=c++17
EMODPS = ./src/moeaframework

all: RICE50++

RICE50++: ./src/RICE.o ./src/RICEMain.o
		${CXX} ${ARGS} ./src/RICEMain.o ./src/RICE.o \
		./src/carbon/Carbon.o ./src/climate/Climate.o \
		./src/economy/Econ.o ./src/economy/EconAgent.o \
		./src/moeaframework/moeaframework.o ./src/moeaframework/param_function.o \
		./src/moeaframework/utils.o ./src/moeaframework/ncRBF.o \
		./src/moeaframework/ann.o ./src/moeaframework/rbf.o \
		./src/moeaframework/ann_mo.o ./src/moeaframework/pwLinear.o \
		 -o RICE50++

./src/RICEMain.o: ./src/RICEMain.cpp ${EMODPS}/moeaframework.o
	${CXX} ${CXXFLAGS} ./src/RICEMain.cpp -o ./src/RICEMain.o

./src/RICE.o: ./src/RICE.cpp ./src/RICE.h ./src/carbon/Carbon.o ./src/climate/Climate.o ./src/economy/Econ.o
	${CXX} ${CXXFLAGS} ./src/RICE.cpp -o ./src/RICE.o

./src/carbon/Carbon.o: ./src/carbon/Carbon.cpp ./src/carbon/Carbon.h
	${CXX} ${CXXFLAGS} ./src/carbon/Carbon.cpp -o ./src/carbon/Carbon.o

./src/climate/Climate.o: ./src/climate/Climate.cpp ./src/climate/Climate.h
	${CXX} ${CXXFLAGS} ./src/climate/Climate.cpp -o ./src/climate/Climate.o

./src/economy/Econ.o: ./src/economy/Econ.cpp ./src/economy/Econ.h ./src/economy/EconAgent.o 
	${CXX} ${CXXFLAGS} ./src/economy/Econ.cpp -o ./src/economy/Econ.o

./src/economy/EconAgent.o: ./src/economy/EconAgent.cpp ./src/economy/EconAgent.h \
	${EMODPS}/param_function.o ${EMODPS}/utils.o \
	${EMODPS}/ncRBF.o ${EMODPS}/ann.o ${EMODPS}/pwLinear.o \
	${EMODPS}/rbf.o ${EMODPS}/ann_mo.o
	${CXX} ${CXXFLAGS} ./src/economy/EconAgent.cpp -o ./src/economy/EconAgent.o

ncRBF.o: $(EMODPS)/ncRBF.cpp $(EMODPS)/ncRBF.h $(EMODPS)/param_function.h
	$(CXX) ${ARGS} -c $(EMODPS)/ncRBF.cpp -o $(EMODPS)/ncRBF.o

ann.o: $(EMODPS)/ann.cpp $(EMODPS)/ann.h $(EMODPS)/param_function.h
	$(CXX) ${ARGS} -c $(EMODPS)/ann.cpp -o $(EMODPS)/ann.o

rbf.o: $(EMODPS)/rbf.cpp $(EMODPS)/rbf.h $(EMODPS)/param_function.h
	$(CXX) ${ARGS} -c $(EMODPS)/rbf.cpp -o $(EMODPS)/rbf.o

pwLinear.o: $(EMODPS)/pwLinear.cpp $(EMODPS)/pwLinear.h $(EMODPS)/param_function.h
	$(CXX) ${ARGS} -c $(EMODPS)/pwLinear.cpp -o $(EMODPS)/pwLinear.o

ann_mo.o: $(EMODPS)/ann_mo.cpp $(EMODPS)/ann_mo.h $(EMODPS)/param_function.h
	$(CXX) ${ARGS} -c $(EMODPS)/ann_mo.cpp -o $(EMODPS)/ann_mo.o


param_function.o: $(EMODPS)/param_function.cpp $(EMODPS)/param_function.h
	$(CXX) ${ARGS} -c $(EMODPS)/param_function.cpp -o $(EMODPS)/param_function.o

utils.o: $(EMODPS)/utils.cpp $(EMODPS)/utils.h
	$(CXX) ${ARGS} -c $(EMODPS)/utils.cpp -o $(EMODPS)/utils.o

moeaframework.o: $(EMODPS)/moeaframework.c $(EMODPS)/moeaframework.h
	${CC} ${ARGS} -c $(EMODPS)//moeaframework.c -o $(EMODPS)//moeaframework.o

clean: 
	rm -rf *.o ./src/climate/*.o ./src/carbon/*.o ./src/economy/*.o ./src/*.o $(EMODPS)/*.o
	rm -rf RICE50++
