#include "Time.h"
#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
	double totalTime = get_wall_time();

	string inputFileName = argv[1];
	string outputFileName = argv[2];

	map<int, vector<int> > communityMAP;
	FILE *f = fopen(inputFileName.c_str(), "r");
	int id, com;
	while ((fscanf(f, "%d %d\n", &id, &com)) != EOF) {
		id = id - 1;
		communityMAP[com].push_back(id);
	}
	fclose(f);

	FILE *f1 = fopen(outputFileName.c_str(), "wt");
	for (map<int,vector<int> >::iterator it = communityMAP.begin(); it != communityMAP.end(); ++it) {
		for (int i = 0; i < it->second.size(); i++) {
			fprintf(f1, "%d ", it->second[i]);
		}
		fprintf(f1, "\n");
	}
	fclose(f1);
    
	return 0;
}
