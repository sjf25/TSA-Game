#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cassert>
#include "Object.h"
#include "Ship.h"
#include "Projectile.h"
#include "utils.h"
//#include <chaiscript/chaiscript.hpp>
//#include <chaiscript/chaiscript_stdlib.hpp>
#include <iostream>
#include <deque>

//Assimp::Importer importer;

const int USER_TEAM = 0;
const int ENEMY_TEAM = 1;

std::vector<Object> field;
std::vector<Object> objects;
std::vector<Ship> ships;
std::deque<Projectile> projectiles;
//std::vector<Ship> teamShips;
//std::vector<Ship> enemyShips;
SDL_Window* window = nullptr;
SDL_GLContext glContext;
Ship user(0, 0, 0, 5, USER_TEAM);

bool setupSDL()
{
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
		return false;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	// TODO make a better title
	window = SDL_CreateWindow("Space Wars", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL);
	if(window == nullptr)
		return false;
	glContext = SDL_GL_CreateContext(window);
	if(glContext == NULL)
		return false;
	// add SDL_GL_SetSwapInterval
	if(TTF_Init() != 0)
		return false;
	return true;
}

void cleanup()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void drawCrossHair()
{
	glColor3f(0, 1, 0);
	drawCircle2f(0, 0, .1, GL_LINE_LOOP);
	glBegin(GL_LINES);
	glVertex2f(.1, 0);	glVertex2f(-.1, 0);
	glVertex2f(0, .1);	glVertex2f(0, -.1);
	glEnd();
	glColor3f(1, 1, 1);
}

double moveTemp = 0;
void draw()
{
	glLoadIdentity();
	
	//std::cout << "DRAWING" << std::endl;
	//glLoadIdentity();
	
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	gluPerspective(45, 640.0/480.0, .001, 100);
	//glTranslatef(user.x,user.y,0);
	//glTranslatef(0, 0, -20);
	glRotatef(user.xRotation, 1, 0, 0);
	glRotatef(user.yRotation, 0, 1, 0);
	glTranslatef(user.x, user.y, user.z);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// try to replace with glFrustum
	//gluPerspective(45, 640.0/480.0, .001, 100);
	
	
	for(auto obj : field)
		obj.draw();
	
	for(auto obj : ships)
	{
		//std::cout << "DRAW" << std::endl;
		obj.draw();
		//Object("Ship", "ship.obj").draw();
	}
	glDisable(GL_TEXTURE_2D);
	for(auto proj : projectiles)
		proj.draw();
	glPopMatrix();
	
	glPushMatrix();
		glOrtho(-1, 1, -1, 1, -100, 100);
		glDisable(GL_TEXTURE_2D);
		drawCrossHair();
		glEnable(GL_TEXTURE_2D);
		static GLuint scoreTex = SOIL_load_OGL_texture("score.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_COMPRESS_TO_DXT);
		static GLuint levelTex = SOIL_load_OGL_texture("level.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_COMPRESS_TO_DXT);
		glBindTexture(GL_TEXTURE_2D, scoreTex);
		glBegin(GL_QUADS);
		
			glTexCoord2f(0, 0);	glVertex2f(-1, 1);
			glTexCoord2f(1, 0);	glVertex2f(-.8, 1);
			glTexCoord2f(1, 1);	glVertex2f(-.8, .8);
			glTexCoord2f(0, 1);	glVertex2f(-1, .8);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, levelTex);
		glBegin(GL_QUADS);
		
			glTexCoord2f(1, 0);	glVertex2f(.8, 1);
			glTexCoord2f(0, 0);	glVertex2f(.6, 1);
			glTexCoord2f(0, 1);	glVertex2f(.6, .8);
			glTexCoord2f(1, 1);	glVertex2f(.8, .8);
		glEnd();
	glPopMatrix();
	SDL_GL_SwapWindow(window);
}

unsigned int drawCallback(unsigned int x, void* param)
{
	SDL_Event e;
	e.type = SDL_WINDOWEVENT;
	SDL_PushEvent(&e);
	//draw();
	return x;
}

void level(int numAstr)
{
	bool running = true;
	
	int points = 0;
	
	//chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());
	//chai.add(chaiscript::user_type<Object>(), "Blah");
	//chai.add(chaiscript::constructor<Object(std::string, std::string)>(), "Blah");
	
	//objects.push_back(Object("Ship", "ship.obj"));
	Ship defShip = Ship("Asteroid", "astr.obj", 0, 0, 0, 0, ENEMY_TEAM);
	for(int i = 0; i < numAstr; i++)
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
	SDL_TimerID timerID = SDL_AddTimer(60, drawCallback, NULL);
	unsigned int prevTime = SDL_GetTicks();
	while(running)
	{
		unsigned int newTime = SDL_GetTicks();
		user.move(newTime - prevTime);
		for(int i = 0; i < projectiles.size(); i++)
		{
			projectiles[i].move(newTime - prevTime);
		}
		while(projectiles.size() > 0 && newTime - projectiles.front().timeSinceCreation >= 3000)
			projectiles.pop_front();
		for(int i = 0; i < ships.size(); i++)
		{
			ships[i].move(newTime - prevTime);
			if( sqrt( pow(ships[i].x + user.x, 2) + pow(ships[i].y + user.y, 2) +
				pow(ships[i].z + user.z, 2) ) < 2)
			{
				points = (points - 1 < 0) ? 0 : points-1;
				ships[i].x = rand() % 20 - 10;
				ships[i].y = rand() % 20 - 10;
				ships[i].z = - (rand() % 100);
				std::cout << "BOOM" << std::endl;
				std::cout << "POINTS: " << points << std::endl;
			}
			// start of test to face enemy
			//ships[i].yRotation = atan2(user.x, user.z) * 180/3.1415;
			//projectiles.push_back(Projectile(ships[i], 10));
			// end of test to face enemy
			for(int j = 0; j < projectiles.size(); j++)
			{
				double distance = sqrt( pow(ships[i].x + projectiles[j].x, 2) + pow(ships[i].y + projectiles[j].y, 2) +
					pow(ships[i].z + projectiles[j].z, 2));
				//std::cout << "Distance: " << distance << std::endl;
				if(distance < 1 && ships[i].teamId != projectiles[j].teamId)
				{
					//std::cout << "Hit" << std::endl;
					//std::cout << "i: " << i << std::endl;
					//std::cout << "j: " << j << std::endl;
					//projectiles.erase(projectiles.begin()+j);
					ships[i].x = rand() % 20 - 10;
					ships[i].y = rand() % 20 - 10;
					ships[i].z = - (rand() % 100);
					points++;
					std::cout << "POINTS: " << points << std::endl;
					//ships.erase(ships.begin()+i);
					break;
					//goto endProjLoop;
				}
			}
			//endProjLoop: ;
		}
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_WINDOWEVENT:
				draw();
				break;
			case SDL_KEYDOWN:
				// TODO fix staggered rotation (make rotation smooth)
				switch(e.key.keysym.sym)
				{
				case SDLK_UP:
					user.xRotation -= 5;
					user.xRotation = fmod(user.xRotation, 360.0);
					if(user.xRotation < -90)
						user.xRotation = -90;
					break;
				case SDLK_DOWN:
					user.xRotation += 5;
					user.xRotation = fmod(user.xRotation, 360.0);
					if(user.xRotation > 90)
						user.xRotation = 90;
					break;
				case SDLK_RIGHT:
					user.yRotation += 5;
					user.yRotation = fmod(user.yRotation, 360.0);
					break;
				case SDLK_LEFT:
					user.yRotation -= 5;
					user.yRotation = fmod(user.yRotation, 360.0);
					break;
				// TODO add time delay btwn shots
				case SDLK_SPACE:
					std::cout << "Shot Fired" << std::endl;
					//projectiles.push_back(Projectile(user.x, user.y, user.z, 10, user.xRotation, user.yRotation));
					projectiles.push_back(Projectile(user, 10));
					break;
				}
				std::cout << user.x << " " << user.y << " " << user.z << std::endl;
				std::cout << user.xRotation << " " << user.yRotation << std::endl;
				//draw();
			}
		}
		prevTime = newTime;
	}
	projectiles.clear();
	ships.clear();
}


std::vector<Ship> harshContain;
void startScreenDraw()
{
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		gluPerspective(45, 640.0/480.0, .001, 100);
		glTranslatef(0, 0, -5);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		harshContain[0].draw();
	glPopMatrix();
	
	glPushMatrix();
		glOrtho(-1, 1, -1, 1, -100, 100);
		static GLuint startTex = SOIL_load_OGL_texture("start.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y| SOIL_FLAG_COMPRESS_TO_DXT);
		//GLuint startTex = createText("ABCDE");
		glBindTexture(GL_TEXTURE_2D, startTex);
		//glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);	glVertex2f(-1, -1);
			glTexCoord2f(1, 0);	glVertex2f(1, -1);
			glTexCoord2f(1, 1);	glVertex2f(1, -.8);
			glTexCoord2f(0, 1);	glVertex2f(-1, -.8);	
		glEnd();
	glPopMatrix();
	SDL_GL_SwapWindow(window);
}

unsigned int startDrawCallback(unsigned int x, void* param)
{
	harshContain[0].yRotation += 2;
	SDL_Event e;
	e.type = SDL_WINDOWEVENT;
	SDL_PushEvent(&e);
	//draw();
	return x;
}

void startScreen()
{
	harshContain.push_back(Ship("HarshShip", "HarshShip.obj", 0, 0, 0, 0, ENEMY_TEAM));
	SDL_TimerID timerID = SDL_AddTimer(60, startDrawCallback, NULL);
	bool running = true;
	SDL_Event event;
	while(running)
	{
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_RETURN)
				{
					std::cout << "Here" << std::endl;
					running = false;
					SDL_RemoveTimer(timerID);
					//return;
				}
				break;
			case SDL_WINDOWEVENT:
				startScreenDraw();
			}
		}
	}
}

int main(int argc, char** argv)
{
	setupSDL();
	glEnable(GL_DEPTH_TEST);
	//gluPerspective(45, 640.0/480.0, .001, 100);
	startScreen();
	level(10);
	cleanup();
}
