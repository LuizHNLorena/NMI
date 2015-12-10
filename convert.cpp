#include "Time.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

int main(int argc, char** argv)
{	
	double timeTotal = get_wall_time();

	string inputFileName = argv[1];
	string outputFileName = argv[2];
	
	string line;
	ifstream inputFile(inputFileName);

	int nodeID, newID=0;
	vector<vector<int >> oldIDS;
	set<int> IDS;

	// READ ORIGINAL PARTITION
	if (inputFile) {
		if (inputFile.is_open())
		{
			while (getline(inputFile, line))
			{
				vector<int> ids;
				stringstream linestream(line);
				while (linestream >> nodeID) {
					ids.push_back(nodeID);
					IDS.insert(nodeID);
				}
				oldIDS.push_back(ids);
			}
			inputFile.close();
		}else {
			cout << "Error opening: " << inputFileName << endl;
			return 0;
		}
	}else {
		cout << "File not found: " << inputFileName << endl;
		return 0;
	}

	// MAP OLD-NEW IDS
	map<int, int> newIDS;
	int ind = 0;
	for (set<int>::iterator it = IDS.begin(); it != IDS.end(); ++it) {
		newIDS[*it] = ind;
		ind++;
	}

	// SAVE NEW CONVERTED PARTITION
	ofstream outputFile;
	outputFile.open(outputFileName);
	if(outputFile.is_open()){
		for (int i = 0; i < oldIDS.size(); i++) {
			for (int j = 0; j < oldIDS[i].size() ;j++) {
				outputFile << newIDS[oldIDS[i][j]] << " ";
			}
			outputFile << "\n";
		}
		outputFile.close();
	}

	cout << "TOTAL TIME (wall clock): " << get_wall_time() - timeTotal << endl;

    return 0;
}
