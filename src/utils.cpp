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
	for(double theta = 0; theta <= 2*3.1415; theta += .2)
	{
		glBegin(GL_POLYGON);
		for(double phi = 0; phi <= 3.1415; phi += .2)
		{
			double xCoord = radius * cos(theta) * sin(phi);
			double yCoord = radius * sin(theta) * sin(phi);
			double zCoord = radius * cos(phi);
			glVertex3f(xCoord+x, yCoord+y, zCoord+z);
		}
		glEnd();
	}
}

GLuint createText(std::string msg)
{
	GLuint texId;
	TTF_Font *font = nullptr;
	font = TTF_OpenFont("font.ttf", 28);
	if(font == nullptr)
		std::cout << "TTF ERROR: " << TTF_GetError() << std::endl;
	SDL_Surface *surface = TTF_RenderText_Blended(font, msg.c_str(), {0, 0, 255});
	
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	int w = toPowOfTwo(surface->w)*2;
	int h = toPowOfTwo(surface->h)*2;
	
	SDL_Surface *newSurface = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_BlitSurface(surface, NULL, newSurface, NULL);
	
   
	//int mode = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
	
	
	/*glBegin(GL_QUADS);
			glTexCoord2f(0, 0);	glVertex3f(-.25, -.25, -3);
			glTexCoord2f(1, 0);	glVertex3f(.25, -.25, -3);
			glTexCoord2f(1, 1);	glVertex3f(.25, .25, -3);
			glTexCoord2f(0, 1);	glVertex3f(-.25, .25, -3);
		glEnd();*/
	return texId;
}

unsigned int toPowOfTwo(unsigned int x)
{
	unsigned int power = 2, nextX = power*2;
	while((nextX *= 2) <= x)
	{
		power *= 2;
	}
	return power * 2;
}
