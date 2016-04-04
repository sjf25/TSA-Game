#include "GL/gl.h"
#include <cmath>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

void drawCircle3f(double x, double y, double z, double radius);
void drawCircle2f(double x, double y, double radius, GLenum mode);
void drawSphere(double, double, double, double);
GLuint createText(std::string);
unsigned int toPowOfTwo(unsigned int x);
