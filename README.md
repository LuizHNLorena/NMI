# NMI

Read ground true and heuristic community files and calculate NMI (normalized Mutual Information) as defined by Danon et al (2005)

# Compiling



# Usage
  
	Usage: " << prog_name << " -g groundTrue_file -h heuristic_file -n num_nodes" << std::endl;
	-g ground true file
	-h heuristic file
	-n Number of nodes in the graph
	
	Return: NMI time
	NMI = -2 * I(X;Y)/(H(X)+H(Y))
	time = wall time in seconds
