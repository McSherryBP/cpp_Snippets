#ifndef BPACKET_H
#define BPACKET_H

#include <vector>
using namespace std;

class Bpacket
{
private:
	float packetSize;
	int destination;
	int origin;
	int delayWait=0;
	int path[150]={-1};

public:
	Bpacket(float size, int dest, int org);
	
	float getPacketSize() {return packetSize;}
	int getDestination() {return destination;}
	int getOrigin() {return origin;}
	void setOrigin(int newO);
	void setDest(int newD);
	void setSize(float newS);
	void setDelay(int delay);
	void tickDelay();
};

#endif
