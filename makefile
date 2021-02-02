CC = gcc
CXX = g++
ARGS = -O3 -Wall
CXXFLAGS = -c -O3 -Wall
MOEAF = ./src/moeaframework

all: RICE50++

RICE50++: ./src/RICE.o ./src/RICEMain.o \
		${MOEAF}/moeaframework.o 
	${CXX} ${ARGS} ./src/RICEMain.o ./src/RICE.o \
		./src/carbon/Carbon.o ./src/climate/Climate.o \
		./src/economy/Econ.o ./src/economy/EconAgent.o \
		./src/moeaframework/moeaframework.o \
		 -o RICE50++

./src/RICEMain.o: ./src/RICEMain.cpp
	${CXX} ${CXXFLAGS} ./src/RICEMain.cpp -o ./src/RICEMain.o

./src/RICE.o: ./src/RICE.cpp ./src/RICE.h ./src/carbon/Carbon.o ./src/climate/Climate.o ./src/economy/Econ.o
	${CXX} ${CXXFLAGS} ./src/RICE.cpp -o ./src/RICE.o

./src/carbon/Carbon.o: ./src/carbon/Carbon.cpp ./src/carbon/Carbon.h
	${CXX} ${CXXFLAGS} ./src/carbon/Carbon.cpp -o ./src/carbon/Carbon.o

./src/climate/Climate.o: ./src/climate/Climate.cpp ./src/climate/Climate.h
	${CXX} ${CXXFLAGS} ./src/climate/Climate.cpp -o ./src/climate/Climate.o

./src/economy/Econ.o: ./src/economy/Econ.cpp ./src/economy/Econ.h ./src/economy/EconAgent.o 
	${CXX} ${CXXFLAGS} ./src/economy/Econ.cpp -o ./src/economy/Econ.o

./src/economy/EconAgent.o: ./src/economy/EconAgent.cpp ./src/economy/EconAgent.h 
	${CXX} ${CXXFLAGS} ./src/economy/EconAgent.cpp -o ./src/economy/EconAgent.o

moeaframework.o: $(MOEAF)/moeaframework.c $(MOEAF)/moeaframework.h
	${CC} ${ARGS} -c $(MOEAF)/moeaframework.c -o $(MOEAF)/moeaframework.o
clean: 
	rm -rf *.o ./src/climate/*.o ./src/carbon/*.o ./src/economy/*.o ./src/*.o $(MOEAF)/*.o
	rm -rf RICE50++
