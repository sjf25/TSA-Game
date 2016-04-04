#include "Object.h"
#include "Ship.h"
#include "utils.h"
#include <SDL2/SDL.h>

class Projectile : public Object
{
public:
	int teamId;
	double speed;
	unsigned int timeSinceCreation = 0;
	bool visible = true;
	//Projectile(double, double, double, double, double, double);
	Projectile(Ship, double);
	void move(int);
	void draw();
};
