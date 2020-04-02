#include "Bnode.h"
#include "Bpacket.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

// Constructor
Bnode::Bnode(int nodeID)
{
	nodeNum=nodeID;
	vector<Bnode> links;
	vector<float> linkBandwidths;
	vector<int> linkDelays;
	vector<Bpacket> sendQueue;
	vector<Bpacket> receiveQueue;
	int nextTarget=-1;
	int toSend=0;
	int toReceive=0;

	bool source=false;
	bool dest=false;
	
	int numberDrops=0;
	int numberSuccess=0;

	int numberConnections=0;
	int rTable[150][2]={};
	// declareState();
}

int Bnode::findRoute(){

	//cout << "Not HERE";
	vector<int> path;
	int next=sendQueue[0].getDestination();
	int depth=rTable[next][0];
	//cout << "HERE";
	for(int i=depth; i>0; i--){
		next=rTable[next][1];
		depth=rTable[next][0];

		cout << next << "|" << depth <<endl;
	}
	// nextTarget=path[path.size()-1];
	return next;
}


// Member Functions
void Bnode::addConnection(Bnode newConnection, float bandwidth, int delay){
	links.push_back(newConnection);
	linkBandwidths.push_back(bandwidth);
	linkDelays.push_back(delay);
	increaseNumConnections();
	// cout << "Node "<<getNodeNum()<<" attempting connection with node "<<newConnection.getNodeNum() << "..." << endl;	
}

void Bnode::declareState(){
	cout << "Node " << getNodeNum() << " online!" << endl;
}

void Bnode::increaseSend(){
	toSend++;
	// cout << "Send increased" <<endl;
}

void Bnode::increaseReceive(){
	toReceive++;
}

void Bnode::decreaseSend(){
	toSend--;
	// cout << "Send increased" <<endl;
}

void Bnode::decreaseReceive(){
	toReceive--;
}


Bpacket Bnode::transferPacket(){
	Bpacket packoLanturn(sendQueue[0].getPacketSize(), sendQueue[0].getDestination(), sendQueue[0].getOrigin());
	sendQueue.erase(sendQueue.begin() + 0);
	decreaseSend();
	// cout << "Transfer";

	return packoLanturn;
}

void Bnode::moveToOutput(){
	Bpacket packoLanturn(receiveQueue[0].getPacketSize(), receiveQueue[0].getDestination(), receiveQueue[0].getOrigin());
	receiveQueue.erase(receiveQueue.begin() + 0);
	decreaseReceive();
	sendPacket(packoLanturn);
}

int Bnode::getSendRemaining(){
	return toSend;
}

int Bnode::getReceiveRemaining(){
	return toReceive;
}

void Bnode::dropPacket(){
	numberDrops++;
}

int Bnode::getDrops(){
	return numberDrops;
}

void Bnode::receivePacket(Bpacket newPack){
	
	if(newPack.getDestination()==nodeNum)
	{
		numberSuccess++;
	}
	else{
		if(receiveQueue.size()<30){
			receiveQueue.push_back(newPack);
			increaseReceive();
		}
		else{
			dropPacket();
		}
	}
	// cout << "receive";
}

void Bnode::sendPacket(Bpacket newPack){
	// cout << "Before send!" <<endl;
	if(sendQueue.size()<30){
		sendQueue.push_back(newPack);
		increaseSend();
	}
	else{
		dropPacket();
	}
}

void Bnode::setTable(int table[][2]){
	for(int i=0; i<150; i++){
		for (int j=0; j<2; j++){
			rTable[i][j]=table[i][j];
		}
		// cout << "Node "<< i<< " at depth "<< rTable[i][0] << " with previous node: " << rTable[i][1]<< endl;
	}
}

void Bnode::generatePacket(int source, int dest, float size){
	Bpacket packoLanturn(dest, size, source);
	sendPacket(packoLanturn);
}

void Bnode::declareConnected(){
	cout << "Node " << getNodeNum()<<"'s connected nodes:" <<endl;	
	if(links.size()==0){
		cout << "* No connections." <<endl;
	}
	else{
		for(int i=0; i<links.size(); i++){
			cout << "* Node " << links[i].getNodeNum()<< " - Bandwidth: "<< linkBandwidths[i]<<endl;
		} // end for
	cout << endl;
	} // end else
}

