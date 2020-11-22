CC = gcc
CXX = g++
ARGS = -O1 -Wall

all: RICE50++

RICE50++: ./src/RICEMain.cpp ./src/RICE.cpp ./src/Carbon.cpp ./src/Climate.cpp ./src/Econ.cpp ./src/EconAgent.cpp 
	${CXX} ${ARGS} ./src/RICEMain.cpp ./src/RICE.cpp ./src/Carbon.cpp ./src/Climate.cpp ./src/Econ.cpp ./src/EconAgent.cpp -o RICE50++

clean: 
	rm RICE50++
