#include "Bpacket.h"


// Constructor
Bpacket::Bpacket(float size, int dest, int org)
{
	float packetSize=size;
	int destination=dest;
	int origin=org;
	int delayWait=0;
	int path[150] ={-1};
	int hopsRemaining=0;
}

void Bpacket::setOrigin(int newO){
	origin=newO;
}

void Bpacket::setSize(float newS){
	packetSize=newS;
}

void Bpacket::setDest(int newD){
	destination=newD;
}

void Bpacket::setDelay(int delay){
	delayWait=delay;
}

void Bpacket::tickDelay(){
	delayWait--;
}
