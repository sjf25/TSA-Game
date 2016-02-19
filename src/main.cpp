#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cassert>
//#include "Object.h"
#include "Ship.h"
//#include <chaiscript/chaiscript.hpp>
//#include <chaiscript/chaiscript_stdlib.hpp>
#include <iostream>

//Assimp::Importer importer;

std::vector<Object> objects;
std::vector<Ship> ships;
SDL_Window* window = nullptr;
SDL_GLContext glContext;
Ship user(0, 0, 0, 1);

bool setupSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return false;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	window = SDL_CreateWindow("Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
	if(window == nullptr)
		return false;
	glContext = SDL_GL_CreateContext(window);
	if(glContext == NULL)
		return false;
	// add SDL_GL_SetSwapInterval
	return true;
}

void cleanup()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

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

void drawCrossHair()
{
	glColor3f(0, 1, 0);
	drawCircle2f(0, 0, .1, GL_LINE_LOOP);
	glColor3f(1, 1, 1);
}

void movePlayer()
{
}

double moveTemp = 0;
void draw()
{
	//glLoadIdentity();
	
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	//glTranslatef(user.x,user.y,0);
	//glTranslatef(0, 0, -20);
	glRotatef(user.xRotation, 1, 0, 0);
	glRotatef(user.yRotation, 0, 1, 0);
	glTranslatef(0, 0, moveTemp);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// try to replace with glFrustum
	//gluPerspective(45, 640.0/480.0, .001, 100);
	
	for(auto obj : ships)
	{
		//std::cout << "DRAW" << std::endl;
		obj.draw();
		//Object("Ship", "ship.obj").draw();
	}
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glOrtho(-1, 1, -1, 1, -100, 100);
	drawCrossHair();
	glPopMatrix();
	SDL_GL_SwapWindow(window);
}

int main(int argc, char** argv)
{
	setupSDL();
	glEnable(GL_DEPTH);
	gluPerspective(45, 640.0/480.0, .001, 100);
	bool running = true;
	
	//chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());
	//chai.add(chaiscript::user_type<Object>(), "Blah");
	//chai.add(chaiscript::constructor<Object(std::string, std::string)>(), "Blah");
	
	//objects.push_back(Object("Ship", "ship.obj"));
	Ship defShip = Ship("Ship", "ship.obj", 0, 0, 0, 1);
	for(int i = 0; i < 50; i++)
	{
		double x, y, z;
		x = rand() % 20 - 10;
		y = rand() % 20 - 10;
		z = - (rand() % 100);
		defShip.x = x;
		defShip.y = y;
		defShip.z = z;
		ships.push_back(defShip);
	}
	//ships.push_back(Ship("Ship", "ship.obj", 0, 0, -5, 1));
	draw();
	
	SDL_GL_SwapWindow(window);
	SDL_Event e;
	
	//double z = 0;
	while(running)
	{
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				switch(e.key.keysym.sym)
				{
				case SDLK_UP:
					user.xRotation -= .2;
					user.xRotation = fmod(user.xRotation, 360.0);
					break;
				case SDLK_DOWN:
					user.xRotation += .2;
					user.xRotation = fmod(user.xRotation, 360.0);
					break;
				case SDLK_RIGHT:
					user.yRotation += .2;
					user.yRotation = fmod(user.yRotation, 360.0);
					break;
				case SDLK_LEFT:
					user.yRotation -= .2;
					user.yRotation = fmod(user.yRotation, 360.0);
					break;
				case SDLK_SPACE:
					moveTemp += .2;
				}
				std::cout << user.x << " " << user.y << " " << user.z << std::endl;
				draw();
			}
		}
	}
	cleanup();
}
