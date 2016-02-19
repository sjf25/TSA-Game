#include "Object.h"

class Ship : public Object
{
public:
	double speed;
	double xRotation = 0;
	double yRotation = 0;
	Ship(std::string, std::string, double, double, double, double);
	Ship(double, double, double, double);
};
