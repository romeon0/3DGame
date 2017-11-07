#pragma once
#ifndef MODEL_H
#define MODEL_H
#pragma comment(lib, "glew32s.lib")


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#ifndef  STBI_LOADED_
	#define STBI_LOADED_
	#include <stb/stb_image.h>
#endif
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"
#include "shader.h"
#include <string>
#include <vector>
#include <gl/GL.h>
using namespace std;
using namespace Assimp;
using namespace glm;



class Model
{
private:
	/*  Model Data */
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;
	glm::mat4 matrix;
	float width = 0, height = 0, depth = 0;
	float x = 1000, y = 1000, z = 1000;
	string name;

	/*  Functions   */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void recalculateMeshVolumes(float x, float y, float z);
	void loadModel(string const &path);
	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

public:
	Model();
	Model(string const &path, bool gamma = false);
	~Model();

	//getters
	glm::mat4 getMatrix();
	float getWidth();
	float getHeight();
	float getDepth();
	glm::vec3 getVolume();
	glm::vec3 getCoords();
	string getName();
	vector<unsigned int> getIndices();
	vector<Vertex> getVertices();

	//setters
	void setCoords(glm::vec3 coord);
	void setName(string n);

	//other
	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma=false);
	void scale(float x, float y, float z);
	void translate(float x, float y, float z);
	void goTo(float x, float y, float z);
	mat4 fakeTranslate(float x, float y, float z);
	void draw(Shader shader);// draws the model, and thus all its meshes
	void extractData(string const &path, bool gamma = false);//if used empty constructor
};
#endif
