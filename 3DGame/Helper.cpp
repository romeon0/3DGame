#include "Helper.h"
#include "Tile.h"
#include "GameWorld.h"
#include <glm/vec3.hpp>
#include <iostream>
#include<assimp/matrix4x4.h>
using std::cout;
using std::endl;
using glm::vec3;

/*Normalize viewport coordinates*/
glm::vec2 Helper::toNormDevCoords(float viewportX, float viewportY, float scrWidth, float scrHeight) {
	float x, y;
	x = (2.0f * viewportX) / scrWidth - 1.0f;
	y = 1.0f - (2.0f * viewportY) / scrHeight;
	return glm::vec2(x, y);
}

/*Transform terrain coords to tile coord*/
int* Helper::toTileCoords(float terrainX, float terrainZ, float terrainMinX, float terrainMinZ) {
	int x, y;
	float offsetX = abs(terrainX - terrainMinX);
	float offsetZ = abs(terrainZ - terrainMinZ);
	x = (int)(offsetZ / 2);
	y = (int)(offsetX / 2);
	int* result = new int[2];
	result[0] = x;
	result[1] = y;
	return result;
}
/*Transform terrain coords to tile coord*/
vec2 Helper::toTileCoords(vec3 terrainCoords, vec3 terrainMin) {
	int x, y;
	vec3 offset = abs(terrainMin- terrainCoords);
	x = (int)(offset.z / 2);
	y = (int)(offset.x / 2);
	return vec2((int)x, (int)y);
}

/*Transform tile to coord*/
vec3 Helper::toWorldCoords(Tile t, vec3 terrainMin) {
	float x, z;
	x = terrainMin.x + t.getY() * 2 + 1;//^
	z = (terrainMin.z + t.getX() * 2);//>
	return vec3(x, 0, z);
}

/*Searching the value in interval [start, end] that is in interval with epsilon length*/
glm::vec3 Helper::binSearch(vec3 start, vec3 end, int currIteration, int iterations, float epsilon) {
	if (currIteration < iterations) {
		++currIteration;
		vec3 medium = (start + end) * 0.5f;
		if (medium.y == 0.0f)
			return medium;
		else if (medium.y > 0.0f)
			start = medium;
		else
			end = medium;
		if ((std::abs(end.y - start.y)) < epsilon) {
			return start;
		}
		return binSearch(start, end, currIteration, iterations, epsilon);
	}
	else
		return vec3(-1000.0f, 1000.0f, 1000.0f);
}

/*Get tiles that occupied by model*/
vector<Tile> Helper::getModelTiles(Model m) {
	vector<Tile> tiles = getModelTiles(m.getCoords(), m.getVolume());
	return tiles;
}

vector<Tile> Helper::getModelTiles(vec3 coords, vec3 params) {
	/*
	^
	|
	x
	z---->
	*/
	vector<Tile> tiles;
	float x1 = coords.x, x2 = coords.x + params.x;
	float y1 = coords.z, y2 = coords.z + params.z;
	float restX, restY;

	cout << "Params: " << params.x << ", " << params.y << ", " << params.z << endl;
	cout << "Coords: " << coords.x << ", " << coords.y << ", " << coords.z << endl;
	GameWorld& g = GameWorld::getInstance();
	vec3 worldMin = g.getWorldOrigin();

	restX = fmod(x1, 2.0f);
	restY = fmod(y1, 2.0f);

	if (restX == 0) {
		if (restY == 0) {//x and y even number
			x1 += 1; 
			y1 += 1;
			for (float x = x1; x < x2; x += 2) {
				for (float y = y1; y < y2; y += 2) {
					vec2 tile = toTileCoords(vec3(x, 0, y), worldMin);
					tile.x -= 1;
					tile.y -= 1;
					tiles.push_back(tile);
				}
			}
		}
		else //only x even number
		{
			x1 += 1;
			for (float x = x1; x < x2; x += 2) {
				for (float y = y1; y <= y2; y += 2) {
					vec2 tile = toTileCoords(vec3(x, 0, y), worldMin);
					tile.x -= 1;
					tile.y -= 1;
					tiles.push_back(tile);
				}
			}
		}
	}
	else { 
		if (restY == 0) { //only y even number
			y1 += 1;
			for (float x = x1; x <= x2; x += 2) {
				for (float y = y1; y < y2; y += 2) {
					vec2 tile = toTileCoords(vec3(x, 0, y), worldMin);
					tile.x -= 1;
					tile.y -= 1;
					tiles.push_back(tile);
				}
			}
		}
		else //both x1 and y1 odd number
		{
			for (float x = x1; x <= x2; x += 2) {
				for (float y = y1; y <= y2; y += 2) {
					vec2 tile = toTileCoords(vec3(x, 0, y), worldMin);
					tile.x -= 1;
					tile.y -= 1;
					tiles.push_back(tile);
				}
			}
		}
	}

	return tiles;
}

mat4x4 Helper::assimpMatToGlmMat(aiMatrix4x4 assimpMat) {
	cout << "Assimp matrix: " << endl;
	for (int a = 0; a < 4; ++a) {
		for (int b = 0; b < 4; ++b) {
			cout << assimpMat[a][b] << " ";
		}
		cout << endl;
	}
	cout << "-------------------" << endl;

	mat4 glmMat(assimpMat.a1, assimpMat.a2, assimpMat.a3, assimpMat.a4,
		assimpMat.b1, assimpMat.b2, assimpMat.b3, assimpMat.b4,
		assimpMat.c1, assimpMat.c2, assimpMat.c3, assimpMat.c4,
		assimpMat.d1, assimpMat.d2, assimpMat.d3, assimpMat.d4);

	cout << "Glm matrix: " << endl;
	for (int a = 0; a < 4; ++a) {
		for (int b = 0; b < 4; ++b) {
			cout << glmMat[a][b] << " ";
		}
		cout << endl;
	}
	cout << "-------------------" << endl;

	return glmMat;
}