#ifndef BLINK_H
#define BLINK_H

#include <vector>
using namespace std;
class Blink
{
private:
	int linkedNodes[2];
	int packet[2];
	float bandwidth;
	float delay;

public:
	Blink(int nodeA, int nodeB, float bandwidth, float delay);
	int* getLinked(){ return linkedNodes;}
	int* getPacket(){ return packet;}
	float getBandwidth(){return bandwidth;}
	float getDelay(){return delay;}
};
#endif
