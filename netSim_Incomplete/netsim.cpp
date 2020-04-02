/* * * * * * * * * * * /
*   Brendan McSherry   *
*  CS 584 - Networks 2 *
*      HW1: Netsim     *
*       2/25/2020      *
* * * * * * * * * * * */

#include "Bnode.h"
#include <vector>
#include <iterator>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

int numNodes = 150; // Global nodeCount

vector<double> calcExponentialDistribution(int packetsQueued){ 

	vector<double> packetOrder;
	int hit=0;
	double current;

	default_random_engine generator;
	exponential_distribution<double> distribution(1);

	while(hit<packetsQueued){
		current=distribution(generator);
		if(current<(double)numNodes){
			packetOrder.push_back(current);
			hit++;
		}
	}
	return packetOrder;

}

vector<Bnode> initiateNodes(){ // Create a vector of all nodes
	vector<Bnode> netNodes;

	for(int i = 0; i < numNodes; i++){
		netNodes.push_back(Bnode(i));
	}
	cout << "" << endl;
	return netNodes;
}

float determineBandwidth(){ // Generate bandwidth based on the given requirements
	
	random_device randomizeEngine;
	mt19937 rEng(randomizeEngine());
	uniform_real_distribution<> dist(0, 1);

	return dist(rEng);

	// return (float)(rand()%1000000);
}

int determineDelay(){ // Generate bandwidth based on the given requirements
	return (int)rand()%11;
}

vector<Bnode> randomlyConnectNodes(vector<Bnode> &rawNodes){
	vector<Bnode> connectedNodes;
	int select = 0; // Randomly select a node to connect
	int select2 = 0; // Randomly select a connected node to pair the new node with

	for(int i = 0; i < numNodes; i++){
		select = rand() % (numNodes - i);
		if(i == 0){
			// cout << "Root added" << endl;
			rawNodes[select].setRoot();
			connectedNodes.push_back(rawNodes[select]);
			rawNodes.erase(rawNodes.begin() + select);
		}else{
			// cout << "Iteration " << i << endl;
			select2 = rand() % connectedNodes.size();
			// cout << "Random value generated: " << select2 << endl;
			// cout << connectedNodes[select2].getNodeNum() <<endl;
			connectedNodes[select2].addConnection(rawNodes[select],determineBandwidth(), determineDelay());
			rawNodes[select].addConnection(connectedNodes[select2],determineBandwidth(), determineDelay());
			connectedNodes.push_back(rawNodes[select]);
			rawNodes.erase(rawNodes.begin() + select);
		}
	}
	// cout << "Number of connected nodes: " << connectedNodes.size() << endl;
	// cout << "Number of unconnected nodes: " << rawNodes.size() << endl;
	return connectedNodes;
}

float computeDropRate(int drops, int totalPackets){
	return (float)drops/(float)totalPackets;
}

vector<int> generatePoission(){ // Use poisson method to determine source of data
// The Mean rate we use will be 0.5 as required

	vector<int> sources;
	int hit=0;
	int current=numNodes+1;

	default_random_engine fishGenerator;
	poisson_distribution<int> distribution(0.5);

	while(hit<20){
		current=distribution(fishGenerator);
		if(current<numNodes){
			sources.push_back(current);
			hit++;
		}
	}
	return sources;
}

void writeNodeFile(vector<Bnode> &connected){
	int numFiles = 0;
	string a = "nodeFile.txt";
	ofstream myfile;
	myfile.open(a);
	myfile << to_string(numNodes) + "\n";
	for(int i=0; i < connected.size(); i++){
		for(int j=0; j < connected[i].getLinks().size(); j++){
			myfile << to_string(connected[i].getNodeNum()) + " ";
			myfile << to_string(connected[i].getLinks()[j].getNodeNum()) + "\n";
		}
	}
	myfile.close();
} // End writeNodeFile

vector<Bnode> loadNodeFile(string nodeFile){
	vector<Bnode> rawNodes;
	int first, second;
	int unique=1;

	ifstream myfile;
	myfile.open(nodeFile);

	if(myfile.is_open()){
		string line;
		getline(myfile, line);
		numNodes=stoi(line);
		// cout << "Confirming: " << numNodes<< " lines." <<endl;
		rawNodes = initiateNodes();

		while(getline(myfile, line)) {
			stringstream s_stream(line);
			string substr;
			getline(s_stream, substr, ' ');
			// cout << "Confirming substring1 " << substr <<endl;
			first=stoi(substr);
			getline(s_stream, substr, ' ');
			second=stoi(substr);
			// cout << "Confirming substring2 " << substr <<endl;
			
			for(int i=0; i<rawNodes[first].getLinks().size(); i++)
			{
				if(rawNodes[first].getLinks()[i].getNodeNum() == rawNodes[second].getNodeNum()){
					unique=0;
				}
			}
			if(unique){
				rawNodes[first].addConnection(rawNodes[second],determineBandwidth(), determineDelay());
				// cout << rawNodes[first].getNodeNum() << " addConnection to " << rawNodes[second].getNodeNum() <<endl;
				rawNodes[second].addConnection(rawNodes[first],determineBandwidth(), determineDelay());
				// cout << rawNodes[second].getNodeNum() << " addConnection to " << rawNodes[first].getNodeNum() <<endl;
			}
	
		unique=1;
		} // End while
	}
	myfile.close();
	return rawNodes;
} // End loadNodeFile

void spanConnection(int confirmed[], Bnode current, vector<Bnode> &nodeGraph){

	confirmed[current.getNodeNum()] = 1;
	// cout << "Node " << current.getNodeNum() << " Reached" << endl;
	for(int j = 0; j < current.getLinks().size(); j++){
		if(confirmed[current.getLinks()[j].getNodeNum()] != 1){
			spanConnection(confirmed, nodeGraph[current.getLinks()[j].getNodeNum()], nodeGraph);
		}
	}		
}

bool determineConnected(vector<Bnode> &nodeGraph){ // Switch to iterative
	
	bool verdict = true;
	int confirmed[numNodes] = {0};
	Bnode current= nodeGraph[0];
	spanConnection(confirmed, current, nodeGraph);
	
	for(int i = 0; i < numNodes; i++){
		if(confirmed[i] == 0){
			verdict = false;
			cout << "node " << i << " Not connected!" <<endl;
			i = numNodes;
		}
	}
	return verdict;
}


// Construct our routing table for the current node.
// The table has a layout where each node is given two values [0] as it's depth from current, and
// the previous node on the route between the current node and the target.
// We build our path backwards this way.
void buildTable(int depth, int mapped[][2], Bnode current, int previous, vector<Bnode> &nodeGraph){
		
		if(depth < mapped[current.getNodeNum()][0]){
		// cout << depth <<" less than " << mapped[current.getNodeNum()][0] << endl;
			mapped[current.getNodeNum()][0]=depth;
			mapped[current.getNodeNum()][1]=previous; // first call should give -1 as previous value

			for(int i=0; i<current.getLinks().size(); i++){
				buildTable(depth+1, mapped, nodeGraph[current.getLinks()[i].getNodeNum()], current.getNodeNum(), nodeGraph);
			} // end for
		} // end if
}

void initTable(int mapped[][2]){
	
	for(int i=0; i<numNodes; i++){
		for (int j=0; j<2; j++){
			mapped[i][j]=numNodes;
		}
	}
}

int main(int argc, char *argv[]){
	unsigned int packetsGenerated = 0;
	unsigned int successfulPackets = 0;
	float successPercent = 0;
	float avgTransTime = 0;
	float maxCompletionTime = 0;
	float minCompletionTime = 1000;
	float avgCompletionTime = 0;
	unsigned int maxPacketsDropped = 0;
	unsigned int minPacketsDropped = 100000;
	unsigned int avgPacketsDropped = 0;
	unsigned int clockTime = 1000;
	int rTable[numNodes][2]={};
	int nextHop=-1;
	int currentDest;
	float currentSize;

	bool connect=false;
	unsigned int seed;

	if(argc == 1){ // Mode 1 generate node files
		vector<Bnode> allNodes = initiateNodes();
		vector<Bnode> connectedNodes = randomlyConnectNodes(allNodes);

		/*
		for(int i = 0; i < connectedNodes.size(); i++){
			cout << connectedNodes[i].getNodeNum() << endl;
			cout << connectedNodes[i].getNumConnections()<< endl;
			cout << connectedNodes[i].getLinks().size()<< endl<< endl;
		}*/ //Used for connectivity tests	

		writeNodeFile(connectedNodes);
	} // End file generation condition 
	else{ // Mode 2 runs a simulation using the passed filename
		string str(argv[1]);
		stringstream(str)>>seed;
		srand (seed);
		vector<Bnode> connectedNodes = loadNodeFile(argv[2]);

		for(int i = 0; i < connectedNodes.size(); i++){
			cout << "Node " <<connectedNodes[i].getNodeNum() << ": " <<endl;			
			for(int j = 0; j < connectedNodes[i].getLinks().size(); j++){
				cout << "   Connected to node " <<connectedNodes[i].getLinks()[j].getNodeNum() << endl;
				cout << "   Bandwidth: " <<connectedNodes[i].getBandwidth()[j] << endl;
				cout << "   Delay: " <<connectedNodes[i].getDelay()[j] << endl;
			}
		} // End Show connections loop

		if(determineConnected(connectedNodes)){
			cout << endl << "        !!!! Graph is connected !!!!" << endl;
			connect=true;
		}
		else{
			cout << "Graph is not connected :(" << endl;
			cout << "Simulation terminated." << endl;
		}
		if(connect){
			// Routing table setup
			
			for(int i=0; i< numNodes; i++)
			{
			initTable(rTable);
			buildTable(0, rTable, connectedNodes[i], -1, connectedNodes);
			connectedNodes[i].setTable(rTable);
			}
			
			
			cout << "Depth | Previous" << endl;
			for(int i=0; i<numNodes; i++){
				cout << "Node " <<i <<": " ;
				for (int j=0; j<2; j++){
					cout << rTable[i][j] << " | ";
				}
				cout << endl;
			}

			cout << "Generating source/dest pairs ...." << endl;
			vector<int> sources = generatePoission();
			vector<int> dests = generatePoission();

			for(int i=0; i<sources.size(); i++){
				// cout << "Source: " <<sources[i] <<" Dest: "<<dests[i] <<endl;
				connectedNodes[sources[i]].setSource();
				connectedNodes[dests[i]].setDest();
			}
			/*
			for(int i = 0; i < clockTime; i++){ // Main time loop
			cout << "Iteration #"<<i <<endl;
			//Phase 1: Update queue, packet, and other times
			for(int j = 0; j < connectedNodes.size(); j++){
			cout << "phase 1: " << j<< endl;;
			cout << connectedNodes[j].getSendRemaining() << " remaining "<< endl;
				if(connectedNodes[j].getSendRemaining()>0){
					// cout << "IN IF" ;
					nextHop=connectedNodes[j].findRoute();
					Bpacket packoLanturn = connectedNodes[j].transferPacket();
					connectedNodes[nextHop].receivePacket(packoLanturn);
				}
				//cout << ".";
			}
			
			// Phase 2: Check leading packets delay cooldown, if <=0, then send. // Full queues drop packets.
			for(int j = 0; j < connectedNodes.size(); j++){
	
			cout << "phase 2" <<endl;;
				if(connectedNodes[j].getReceiveRemaining()>0){
					connectedNodes[j].moveToOutput();
				}
				//cout << "+";
			}			

			// Phase 3: Generate packets at random nodes with random size and random destination!
			for(int j =0; j<20; j++){
				cout << "phase 3" <<endl;;
				packetsGenerated++;
				if(connectedNodes[sources[j]].getSendRemaining()<30){
					//cout << "Packet generated " << j<<endl;
				connectedNodes[sources[j]].generatePacket(sources[j], dests[j], determineBandwidth());
					//cout << "Packet added to queue" <<endl;
				}
				else{
					//cout << "Packet not generated: queue full. Packet loss increased." << endl;
					connectedNodes[sources[j]].dropPacket();
				}
				//cout << "$";
			}

			} // End clock loop*/
		}
	// cout <<"Sim ended" <<endl; 
	
	int totalDrop=0;
	for (int i=0; i<numNodes; i++){
		totalDrop += connectedNodes[i].getDrops();
		if(connectedNodes[i].getDrops() > maxPacketsDropped){
			maxPacketsDropped=connectedNodes[i].getDrops();
		}
		if(connectedNodes[i].getDrops() < minPacketsDropped){
			minPacketsDropped=connectedNodes[i].getDrops();
		}
	}
	avgPacketsDropped=totalDrop/numNodes;
	cout <<"Total dropped " <<totalDrop <<endl;

	cout << endl << "* * * * * * Simulation Statistics * * * * * *" << endl << endl;
	cout << "Packets generated: " << to_string(packetsGenerated) << endl;
	cout << "Successful packets: " << to_string(successfulPackets) << endl;
	cout << "Successful packet percentage: " << to_string(successPercent) << "%"<<endl;
	cout << "Average transmission time: " << to_string(avgTransTime) << endl;
	cout << "Maximum completion time: " << to_string(maxCompletionTime) << endl;
	cout << "Minimum completion time: " << to_string(minCompletionTime) << endl;
	cout << "Average completion time: " << to_string(avgCompletionTime) << endl;
	cout << "Greatest number of packets dropped: " << to_string(maxPacketsDropped) << endl;
	cout << "Min number of packets dropped: " << to_string(minPacketsDropped) << endl;
	cout << "Average number of dropped packets: " << to_string(avgPacketsDropped) << endl;
	} // End else 
	return 0;
}
