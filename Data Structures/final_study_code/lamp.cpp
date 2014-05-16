#include <iostream>
#include <string>

#include "lamp.h"

Lamp::Lamp(int n, int watts) : num_bulbs(n){
	sockets = new Bulb[n];
	for(Bulb* p = sockets; p != sockets + n; p++){
		
	}
}