
#pragma comment(lib, "glew32s.lib")
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "mesh.h"
#include "shader.h"
#include <string>
#include <iostream>
#include <vector>
#include "SimpleModel.h"
#include <gl/GL.h>
using namespace std;
using namespace glm;

SimpleModel::SimpleModel(vector<Vertex> vertices, vector<unsigned int> indices, bool hasTexture) {
	this->vertices = vertices;
	this->indices = indices;
	if (this->vertices.size() == 0) {
		Vertex v1, v2, v3;
		v1.Position = vec3(0.0, 0.0, 0.0);
		v2.Position = vec3(0.5, 0.0, 0.0);
		v3.Position = vec3(0.5, 0.5, 0.0);
		this->vertices.push_back(v1);
		this->vertices.push_back(v2);
		this->vertices.push_back(v3);
	}
	if (this->indices.size() == 0) {
		this->indices.push_back(0);
		this->indices.push_back(1);
		this->indices.push_back(2);
	}

	initModel();
}

vector<Vertex> SimpleModel::getVertices() { return vertices; }
vector<unsigned int> SimpleModel::getIndices() { return indices; }
void SimpleModel::setName(string n) { name = n; }

void SimpleModel::draw(Shader shader, GLenum drawMode) {
	if (vertices.size() == 0) { return; }
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
	//glDrawElements(drawMode, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glDrawArrays(drawMode, 0, vertices.size());
	glBindVertexArray(0);
}

void SimpleModel::translate(float x, float y, float z) {
	matrix = glm::translate(matrix, glm::vec3(x, y, z));
	this->x -= x;
	this->y -= y;
	this->z -= z;
}
void SimpleModel::scale(float x, float y, float z) {/*TODO implement function*/}
glm::mat4 SimpleModel::getMatrix() { return matrix; }
float SimpleModel::getWidth() { return width; }
float SimpleModel::getHeight() { return height; }
float SimpleModel::getDepth() { return depth; }
glm::vec3 SimpleModel::getVolume() { return vec3(width, height, depth); }
glm::vec3 SimpleModel::getCoords() { return vec3(x, y, z); }
void SimpleModel::setCoords(glm::vec3 coord) { 
	x = coord.x;
	y = coord.y;
	z = coord.z;
}

void SimpleModel::initModel() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glBindVertexArray(0);
}

void SimpleModel::updateIndices(vector<unsigned int> newIndices) {
	//indices.clear();
	for (uint i : newIndices) {
		//indices.push_back(i);
	}
	indices = newIndices;
}
void SimpleModel::updateVertices(vector<Vertex> newVertices) {
	//vertices.clear();
	for (Vertex v : newVertices) {
		//vertices.push_back(v);
	}
	vertices = newVertices;
	//cout << " Vertices updated: " << newVertices[0].Position.x << "::" << vertices[0].Position.x << endl;
}