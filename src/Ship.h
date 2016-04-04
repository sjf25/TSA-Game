#include "Object.h"

#ifndef SHIP_H
#define SHIP_H
class Ship : public Object
{
public:
	int teamId;
	double speed;
	Ship(std::string, std::string, double, double, double, double, int);
	Ship(double, double, double, double, int);
	Ship();
	void move(int);
};
#endif
