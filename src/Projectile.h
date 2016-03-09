#include "Object.h"
#include "Ship.h"
#include "utils.h"

class Projectile : public Object
{
public:
	double speed;
	double xRotation = 0;
	double yRotation = 0;
	long timeSinceCreation = 0;
	//Projectile(double, double, double, double, double, double);
	Projectile(Ship, double);
	void move(int);
	void draw();
};
