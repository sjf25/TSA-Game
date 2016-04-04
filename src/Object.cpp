#include "Object.h"

Object::Object(std::string filePath, std::string modelName)
{
	loadModel(filePath, modelName);
}

Object::Object(std::string filePath, std::string modelName, double a, double b, double c)
{
	loadModel(filePath, modelName);
	x = a;
	y = b;
	z = c;
}

Object::Object(double a, double b, double c)
{
	x = a;
	y = b;
	z = c;
}

void Object::loadModel(std::string filePath, std::string modelName)
{
	std::string fullModelLocation = filePath + "/" + modelName;
	std::cout << fullModelLocation << std::endl;
	//Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fullModelLocation, aiProcessPreset_TargetRealtime_Fast);
	mesh = scene->mMeshes[0];
	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	aiString texturePath;
	material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
	std::string fullTextureLocation = filePath+"/";
	fullTextureLocation += texturePath.C_Str();
	std::cout << fullTextureLocation << std::endl;
	texture = SOIL_load_OGL_texture(fullTextureLocation.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	
	
	assert(mesh != nullptr);
	//glPushMatrix();
	//glTranslatef(x, y, z);
	//glBindTexture(GL_TEXTURE_2D, texture);
	for(int i = 0; i < mesh->mNumFaces; i++)
	{
		//start of new stuff
		//end of new stuff
		aiFace face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		//glBegin(GL_TRIANGLES);
		for(int j = 0; j < face.mNumIndices; j++)
		{
			int index = face.mIndices[j];
			if(mesh->HasTextureCoords(0))
			{
				//std::cout << "HERE\n";
				//glTexCoord2f(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y);
				textureCoords.push_back({{mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y}});
			}
			//glVertex3fv(&mesh->mVertices[index].x);
			vertexCoords.push_back(std::vector<double>(&mesh->mVertices[index].x, &mesh->mVertices[index].x+3));
		}
		//glEnd();
	}
	//glPopMatrix();
	
	std::cout << "Done Loading" << std::endl;
}

void Object::draw()
{
	glPushMatrix();
	glTranslatef(x, y, z);
	//glRotatef(xRotation, 1, 0, 0);
	glRotatef(yRotation, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLES);
	for(int i = 0; i < vertexCoords.size(); i++)
	{
		glTexCoord2f(textureCoords[i][0], textureCoords[i][1]);
		glVertex3f(vertexCoords[i][0], vertexCoords[i][1], vertexCoords[i][2]);
	}
	glEnd();
	glPopMatrix();
}

/*
void Object::draw()
{
	assert(mesh != nullptr);
	glPushMatrix();
	glTranslatef(x, y, z);
	glBindTexture(GL_TEXTURE_2D, texture);
	for(int i = 0; i < mesh->mNumFaces; i++)
	{
		//start of new stuff
		//end of new stuff
		aiFace face = mesh->mFaces[i];
		//assert(face.mNumIndices == 3);
		//glBegin(GL_TRIANGLES);
		//for(int j = 0; j < face.mNumIndices; j++)
		{
			//int index = face.mIndices[j];
			//if(mesh->HasTextureCoords(0))
			{
				//std::cout << "HERE\n";
				//glTexCoord2f(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y);
			}
			//glVertex3fv(&mesh->mVertices[index].x);
		}
		//glEnd();
	}
	glPopMatrix();
}
*/
