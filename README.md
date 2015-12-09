# NMI

Calculate NMI (normalized Mutual Information) as defined by Danon et al (2005)

# Usage
  
	Usage: ./nmi -g groundTrue_file -h heuristic_file -n num_nodes
	-g ground true file
	-h heuristic file
	-n Number of nodes in the graph
	
	Return: NMI time
	NMI = -2 * I(X;Y)/(H(X)+H(Y))
	time = wall time in seconds

# Community file format

One community per line with the ID of each node separated by space. All node ID needs to  be in range [0,n-1].

The following example presents a partition with 3 commuties and 10 nodes.

	0 4 3 2 7
	1 5 6
	8 9

# Example of use

	./nmi -g groundtruePartition.txt -h heuristicPartition.txt -n 334863
