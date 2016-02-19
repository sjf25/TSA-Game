#include "Ship.h"

Ship::Ship(std::string filePath, std::string modelName, double a, double b, double c, double sp)
	: Object(filePath, modelName, a, b, c)
{
	speed = sp;
}

Ship::Ship(double a, double b, double c, double sp)
	: Object(a, b, c)
{
	speed = sp;
}
