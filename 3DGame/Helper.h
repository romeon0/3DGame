#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <cmath>
#include "Tile.h"
#include "Model.h"
#include <vector>
using glm::vec3;
using glm::vec2;

class Helper {
private:
public:
	/*Normalize viewport coordinates*/
	glm::vec2 toNormDevCoords(float viewportX, float viewportY, float scrWidth, float scrHeight);

	/*Transform terrain coords to tile coord*/
	int* toTileCoords(float terrainX, float terrainZ, float terrainMinX, float terrainMinZ);
	/*Transform terrain coords to tile coord*/
	vec2 toTileCoords(vec3 terrainCoords, vec3 terrainMin);

	/*Transform tile to coord*/
	vec3 toWorldCoords(Tile t, vec3 terrainMin);

	/*Searching the value in interval [start, end] that is in interval with epsilon length*/
	glm::vec3 binSearch(vec3 start, vec3 end, int currIteration, int iterations, float epsilon);

	vector<Tile> getModelTiles(Model m);
	vector<Tile> Helper::getModelTiles(vec3 params, vec3 coords);

	mat4x4 Helper::assimpMatToGlmMat(aiMatrix4x4 assimpMat);
};