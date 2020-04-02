#ifndef BNODE_H
#define BNODE_H

#include "Bpacket.h"
#include <vector>
using namespace std;

class Bnode
{
private:
	int nodeNum;
	bool root=false;
	bool source=false;
	bool dest=false;

	int toSend=0;
	int toReceive=0;
	int nextTarget=-1;

	int numberDrops=0;
	int numberSuccess=0;

	vector<Bnode> links;
	vector<float> linkBandwidths;
	vector<int> linkDelays;
	Bpacket* packets[2][30]; 

	vector<Bpacket> sendQueue;
	vector<Bpacket> receiveQueue;
	int numConnections=0;
	// Nodes can be sources or dests
	int rTable[150][2]={};
	
public:
	Bnode(int nodeID);

	int findRoute();
	void initPackets();
	void receivePacket(Bpacket newPack);
	void sendPacket(Bpacket newPack);
	Bpacket transferPacket();
	void moveToOutput();
	void declareState();
	void declareConnected();
	void addConnection(Bnode newConnection, float bandwidth, int delay);	
	void setRoot(){root=true;}
	void setSource(){source=true;}
	void setDest(){dest=true;}
	void setTable(int table[150][2]);
	void decreaseSend();
	void increaseSend();
	void increaseReceive();
	void decreaseReceive();
	void dropPacket();
	int getDrops();
	void generatePacket(int source, int dest, float size);
	int getNodeNum() {return nodeNum;}
	vector<float> getBandwidth() {return linkBandwidths;}
	vector<int> getDelay() {return linkDelays;}
	vector<Bnode> getLinks(){return links;}
	int getNumConnections(){ return numConnections;}
	void increaseNumConnections(){ numConnections ++;}
	int getSendRemaining();
	int getReceiveRemaining();
};

#endif
