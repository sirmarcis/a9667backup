#ifndef LAMP_H_INCLUDED
#define LAMP_H_INCLUDED


#include <iostream> 
#include <string>
#include <cassert>

class Bulb{
public:
	int getWatts() const { return watts; }
	void setWatts(int w) { watts = w; }
private:
	int watts;
};

class Lamp{
public:
	Lamp(int n , int watts);
	virtual ~Lamp();
	void replace_bulb(int i, int watts) {
		assert (i >= 0 && i < num_bulbs);
		sockets[i].setWatts(watts); }
	int switch_lights_on();
	virtual void switch_off() { lights_on = false; }
private:
	// REPRESENTATION
	int num_bulbs;
	Bulb *sockets; // a dynamically allocated array
	bool lights_on;

};

class FanLamp : public Lamp {
public:
	FanLamp(int n, int watts) : Lamp(n, watts) {}
	~FanLamp() {}
	// MODIFIERS
	void switch_fan_on() { fan_on = true; }
	void switch_off();
private:
	// REPRESENTATION
	bool fan_on;
};


#endif // LAMP_H_INCLUDED
