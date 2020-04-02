#include "Blink.h"
#include <vector>
using namespace std;
// Constructor
Blink::Blink(int nodeA, int nodeB, float band, float d)
{
	int linkedNodes[2];
	int packet[2];
	float bandwidth=band;
	float delay=d;

	linkedNodes[0]=nodeA;
	linkedNodes[1]=nodeB;
	
}
