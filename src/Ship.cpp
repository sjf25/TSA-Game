#include "Ship.h"

Ship::Ship(std::string filePath, std::string modelName, double a, double b, double c, double sp, int id)
	: Object(filePath, modelName, a, b, c)
{
	teamId = id;
	speed = sp;
}

Ship::Ship(double a, double b, double c, double sp, int id)
	: Object(a, b, c)
{
	teamId = id;
	speed = sp;
}

Ship::Ship() : Object(0, 0,0 ) {}

void Ship::move(int milliSecs)
{
	const double ratio = 3.14159/180.0;
	//moveTemp += .2;
	/*double yPrime = user.y * cos(user.xRotation) - user.z * sin(user.xRotation);
	double zPrime = user.y * sin(user.xRotation) + user.z * cos(user.xRotation);
	double xPrime = user.x;
	double zDoublePrime = zPrime * cos(user.yRotation) - xPrime * sin(user.yRotation);
	double xDoublePrime = zPrime * sin(user.yRotation) + xPrime * cos(user.yRotation);
	double yDoublePrime = yPrime;
	user.x = xDoublePrime;
	user.y = yDoublePrime;
	user.z = zDoublePrime;*/
	double deltaY = (milliSecs/1000.0)*speed * sin(xRotation * ratio);
	double deltaX = (milliSecs/1000.0)*speed * sin(yRotation * ratio);
	y += deltaY;
	x -= deltaX;
	double zSign = (yRotation < 90 && yRotation > -90) ? 1:-1;
	z += zSign * sqrt(fabs(pow((milliSecs/1000.0) * speed, 2) - pow(deltaX, 2) - pow(deltaY, 2)));
	//user.z += user.speed * (sin(user.yRotation * ratio) - cos(user.xRotation * ratio));
}
