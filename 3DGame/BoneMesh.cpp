//#pragma comment(lib, "glew32s.lib")
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h> // holds all OpenGL type declarations

#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "BoneMesh.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include<glm/vec4.hpp>
using glm::vec4;
using namespace std;

/*  Functions  */
// constructor
BoneMesh::BoneMesh(vector<BoneVertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
{
	assert(vertices.size() != 0);
	assert(indices.size() != 0);

	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	calculateVolume();
	setupMesh();
}

BoneMesh::~BoneMesh() {
	//TODO implement destructor
	/*cout << "Mesh removed" << endl;
	glActiveTexture(GL_TEXTURE0);
	for (Texture t : textures)
	glDeleteTextures(1, &t.id);
	textures.clear();
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);*/
}

vector<BoneVertex> BoneMesh::getVertices() {
	return vertices;
}
vector<unsigned int> BoneMesh::getIndices() {
	return indices;
}

void BoneMesh::calculateVolume() {
	glm::vec3 min(1000.0, 1000.0, 1000.0);
	glm::vec3 max(0.0, 0.0, 0.0);
	glm::vec3 pos;
	for (int a = 0; a < vertices.size(); ++a) {
		pos = vertices[a].position;
		if (pos.x < min.x)
			min.x = pos.x;
		if (pos.y < min.y)
			min.y = pos.y;
		if (pos.z < min.z)
			min.z = pos.z;

		if (pos.x > max.x)
			max.x = pos.x;
		if (pos.y > max.y)
			max.y = pos.y;
		if (pos.z > max.z)
			max.z = pos.z;
	}
	width = abs(max.x - min.x);
	height = abs(max.y - min.y);
	depth = abs(max.z - min.z);
	x = -min.x;
	y = -min.y;
	z = -min.z;
}
// render the mesh
void BoneMesh::draw(Shader shader)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active before binding
										  // retrieve texture number (the N in diffuse_textureN)
		stringstream ss;
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			ss << diffuseNr++; // transfer unsigned int to stream
		else if (name == "texture_specular")
			ss << specularNr++; // transfer unsigned int to stream
		else if (name == "texture_normal")
			ss << normalNr++; // transfer unsigned int to stream
		else if (name == "texture_height")
			ss << heightNr++; // transfer unsigned int to stream
		number = ss.str();
		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.getId(), (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);


	//set to default
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

glm::vec3 BoneMesh::getVolume() {
	return glm::vec3(width, height, depth);
}
glm::vec3 BoneMesh::getCoords() {
	return glm::vec3(x, y, z);
}

/*  Functions    */
// initializes all the buffer objects/arrays
void BoneMesh::setupMesh()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(BoneVertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BoneVertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BoneVertex), (void*)offsetof(BoneVertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BoneVertex), (void*)offsetof(BoneVertex, texCoords));
	// bone ids
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(BoneVertex), (void*)offsetof(BoneVertex, boneIds));
	// bone weights
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(BoneVertex), (void*)offsetof(BoneVertex, boneWeights));
	
	glBindVertexArray(0);
}

void BoneMesh::addBoneData(unsigned int vertexId, float weight, unsigned int boneId) {
	vec4 weights = vertices.at(vertexId).boneWeights;
	bool found = false;
	for (int a = 0; a < 4; ++a) {
		if (weights[a] == 0.0f) {
			vertices.at(vertexId).boneWeights[a] = weight;
			vertices.at(vertexId).boneIds[a] = boneId;
			found = true;
			break;
		}
	}

	assert(found);//more than 4 bones influences this vertex
}

void BoneMesh::update() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(BoneVertex), &vertices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}