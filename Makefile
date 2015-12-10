all: nmi convert convertLFR

nmi: nmi.cpp
	g++ nmi.cpp -std=c++11 -fopenmp -O3 -o nmi
	
convert: convert.cpp
	g++ convert.cpp -std=c++11 -O3 -o convert

convertLFR: convertLFR.cpp
	g++ convert.cpp -std=c++11 -O3 -o convertLFR
