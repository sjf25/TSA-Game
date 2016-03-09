#include "utils.h"

void drawCircle3f(double x, double y, double z, double radius)
{
	const double pi = 3.14;
	double segCount = 20;
	glBegin(GL_POLYGON);
	for(double i = 0; i < segCount; i++)
	{
		double xCoord = radius * cos(2*pi/segCount * i) + x;
		double yCoord = radius * sin(2*pi/segCount * i) + y;
		glVertex3f(xCoord, yCoord, z);
	}
	glEnd();
}

void drawCircle2f(double x, double y, double radius, GLenum mode = GL_POLYGON)
{
	const double pi = 3.14;
	double segCount = 20;
	glBegin(mode);
	for(double i = 0; i < segCount; i++)
	{
		double xCoord = radius * cos(2*pi/segCount * i) + x;
		double yCoord = radius * sin(2*pi/segCount * i) + y;
		glVertex2f(xCoord, yCoord);
	}
	glEnd();
}

void drawSphere(double x, double y, double z, double radius)
{
	for(double theta = 0; theta <= 2*3.1415; theta += .1)
	{
		glBegin(GL_POLYGON);
		for(double phi = 0; phi <= 3.1415; phi += .1)
		{
			double xCoord = radius * cos(theta) * sin(phi);
			double yCoord = radius * sin(theta) * sin(phi);
			double zCoord = radius * cos(phi);
			glVertex3f(xCoord+x, yCoord+y, zCoord+z);
		}
		glEnd();
	}
}
