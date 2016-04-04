#include "SOIL.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <GL/gl.h>
#include <cassert>
#include <iostream>
#include <vector>

#ifndef OBJECT_H
#define OBJECT_H
class Object
{
private:
	Assimp::Importer importer;
	aiMesh* mesh;
	GLuint texture;
	std::vector<std::vector<double>> textureCoords;
	std::vector<std::vector<double>> vertexCoords;
	void loadModel(std::string, std::string);
public:
	double x=0, y=0, z=0;
	double xRotation = 0;
	double yRotation = 0;
	Object(std::string, std::string);
	Object(std::string, std::string, double, double, double);
	Object(double, double, double);
	void draw();
};
#endif
