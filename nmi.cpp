#ifdef _OPENMP
#include <omp.h>
#endif
#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include "Time.h"

using namespace std;

bool readFiles(const string& fileNameGT,
	             const string& fileNameHEU,
				 const int& n,
				 vector<vector<int> >& confusion_matrix,
				 double& entropyGT,
				 double& entropyHEU,
				 vector<int>& NI, 
	             vector<int>& NJ) {
	
	// READING GROUND TRUE FILE
	int totalComGT = 0; bool errorFileGT = 0; vector<int> communityGT(n); entropyGT = 0.0;
	int nodeId, sizeCom;
	string line;
	ifstream fileGT(fileNameGT);

	if (fileGT) {
		if (fileGT.is_open())
		{
			while (getline(fileGT, line))
			{
				stringstream linestream(line);
				sizeCom = 0;
				while (linestream >> nodeId) {
					sizeCom++;
					communityGT[nodeId] = totalComGT;
				}
				entropyGT = entropyGT + (sizeCom * (log((double)sizeCom / n)));
				totalComGT++;
			}
			fileGT.close();
		}
		else {
			errorFileGT = 1;
		}
	}
	else {
		errorFileGT = 1;
	}

	// READING HEURISTIC FILE
	int totalComHEU = 0; bool errorFileHEU = 0; vector<int> communityHEU(n); entropyHEU = 0.0;
	ifstream fileHEU(fileNameHEU);
	if (fileHEU) {
		if (fileHEU.is_open())
		{
			while (getline(fileHEU, line))
			{
				stringstream linestream(line);
				sizeCom = 0;
				while (linestream >> nodeId) {
					sizeCom++;
					communityHEU[nodeId] = totalComHEU;
				}
				entropyHEU = entropyHEU + (sizeCom * (log((double)sizeCom / n)));
				totalComHEU++;
			}
			fileHEU.close();
		}else {
			errorFileHEU = 1;
		}
	}else {
		errorFileHEU = 1;
	}

	// CHECK FOR FILE ERRORS
	if (errorFileGT || errorFileHEU) {
		if (errorFileGT == 1 && errorFileHEU == 0) {
			std::cout << "Error reading file: " << fileNameGT << std::endl;
		}else if (errorFileGT == 0 && errorFileHEU == 1) {
			std::cout << "Error reading file: " << fileNameHEU << std::endl;
		}else {
			std::cout << "Error reading files: " << fileNameGT << " e " << fileNameHEU << std::endl;
		}
		return 1;
	}

	// CREATE CONFUSION MATRIX
	confusion_matrix.resize(totalComGT, vector<int>(totalComHEU, 0));

	// Ni and Nj
	NI.resize(totalComGT);
	NJ.resize(totalComHEU);

	// POPULATING CONFUSION MATRIX, Ni and Nj
	int i, j;
	int difI, difJ;
	for (int l = 0; l < n; ++l)
	{
		i = communityGT[l];
		j = communityHEU[l];

		confusion_matrix[i][j]++;

		difI = confusion_matrix[i][j] - NI[i];
		if (difI == 0) {
			NI[i] = NI[i] + 1;
		}else {
			NI[i] = NI[i] + (confusion_matrix[i][j] - NI[i]);
		}

		difJ = confusion_matrix[i][j] - NJ[j];
		if (difJ == 0) {
			NJ[j] = NJ[j] + 1;
		}else {
			NJ[j] = NJ[j] + difJ;
		}
	}
	return 0;
}

int usage(char *prog_name, const char *more) {
	std::cerr << more << std::endl;
	std::cerr << "####################################" << std::endl;
	std::cerr << "# Usage: " << prog_name << " -g groundTrue_file -h heuristic_file -n num_nodes" << std::endl;
	std::cerr << "# Description: Read ground true and heuristic community files and calculate NMI." << std::endl;
	std::cerr << "# \t -g ground true file" << std::endl;
	std::cerr << "# \t -h heuristic file" << std::endl;
	std::cerr << "# \t -n Number of nodes in the graph" << std::endl;
	std::cerr << "# Return: NMI time" << std::endl;
	std::cerr << "# \t NMI = -2 * I(X;Y)/(H(X)+H(Y))" << std::endl;
	std::cerr << "# \t time = wall time in seconds" << std::endl;
	std::cerr << "####################################" << std::endl;
	return 0;
}

void parse_args(int argc, char **argv, string& groundTrueFile, string& heuristicFile, int& n) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'g':
				if (i == argc - 1)
					usage(argv[0], "Ground True file missing!\n");
				groundTrueFile = argv[i + 1];
				i++;
				break;
			case 'h':
				if (i == argc - 1)
					usage(argv[0], "Heuristic True file missing!\n");
				heuristicFile = argv[i + 1];
				i++;
				break;
			case 'n':
				if (i == argc - 1)
					usage(argv[0], "Graph total number of nodes missing!\n");
				n = atoi(argv[i + 1]);
				i++;
				break;
			default:
				usage(argv[0], "Unknown option.\n");
			}
		}
	}
}

int main(int argc, char **argv) {

	double wallTime = get_wall_time();
	string fileNameGT;
	string fileNameHEU;
	int n;

	if (argc < 7)
		return usage(argv[0], "Missing mandatory parameters!\n");

	parse_args(argc, argv, fileNameGT, fileNameHEU, n);

	vector<vector<int> > confusion_matrix;
	double entropyGT;
	double entropyHEU;
	vector<int> NI;
	vector<int> NJ;

	// READ FILES, COMPUTE CONFUSION MATRIX AND ENTROPY OF GROUND TRUE AND HEURISTIC PARTITIONS
	if (readFiles(fileNameGT, fileNameHEU, n, confusion_matrix, entropyGT, entropyHEU, NI, NJ)) {
		return 1;
	}
	
	// COMPUTE MUTUAL INFORMATION
	double num = 0;
	for (int i = 0; i < NI.size(); ++i) {
		#pragma omp parallel for reduction (+:num)
		for (int j = 0; j < NJ.size(); ++j)
		{
			if (confusion_matrix[i][j] != 0)
				num += confusion_matrix[i][j] * log((double)(confusion_matrix[i][j] * n) / (NI[i] * NJ[j]));				
		}
	}

	// "NMI" "wall time"
	cout << (-2.0 * num) / (entropyGT + entropyHEU) << " " << get_wall_time() - wallTime << endl;

	return 0; 
}
