# NMI

Calculate NMI (normalized Mutual Information) as defined by Danon et al (2005).
	
	DANON, Leon et al. Comparing community structure identification. 
	Journal of Statistical Mechanics: Theory and Experiment, v. 2005, n. 09, p. P09008, 2005.

# Compiling

Just call `make` in Linux or Mac.

# Usage
  
	Usage: ./nmi -g groundTrue_file -h heuristic_file -n num_nodes
	-g ground true file
	-h heuristic file
	-n Number of nodes in the graph
	
	Return: NMI time
	NMI = -2 * I(X;Y)/(H(X)+H(Y))
	time = wall time in seconds

# Example of use

	./nmi -g groundtruePartition.txt -h heuristicPartition.txt -n 334863

# Community file format

One community per line with the ID of each node separated by space. All node IDs needs to  be in range [0,n-1].

The following example presents a partition with 3 commuties and 10 nodes.

	0 4 3 2 7
	1 5 6
	8 9

If your partition follows the pattern of one community per line but is out of range [0,n-1] you can convert it with the program [convert](convert.cpp).

	./convert originalFile.txt resultingFile.txt

If you are working with [LFR benchmark](https://sites.google.com/site/andrealancichinetti/files) community files you can convert it by using the program [convertLFR](convertLFR.cpp).

	./convertLFR LFRfile.txt resultingFile.txt
