#include "Helper.h"
#include "Tile.h"
#include "GameWorld.h"
#include <glm/vec3.hpp>
#include <iostream>
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
	float offsetX = terrainX - terrainMinX;
	float offsetZ = terrainZ - terrainMinZ;
	x = (int)(offsetZ / 2);
	y = (int)(offsetX / 2);
	int result[2] = { x,y };
	return result;
}
/*Transform terrain coords to tile coord*/
vec2 Helper::toTileCoords(vec3 terrainCoords, vec3 terrainMin) {
	int x, y;
	vec3 offset = abs(terrainCoords - terrainMin);
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
	Tile t(-1, -1);
	float x1 = coords.x, x2 = coords.x + params.x;
	float y1 = coords.z, y2 = coords.z + params.z;
	float xCurr, yCurr;
	bool finish;
	int counter;

	/*
	Ex1:
		Input: coord(3,1,1),volume(3,2,4).
		Body:
			start = coord = 3,1,1
			final = coord+volume = 6,3,5
		Res: nrOfXTiles =  start.x%2==0?0:1 + ((start.x-start.x%2)+2) + ((start.x-start.x%2)+4) = 1+1+1=3;
			 nrOfYTiles =  start.y%2==0?0:1 + ((start.y-start.y%2)+2) + ((start.y-start.y%2)+4) = 1+1+1=3;

	*/
	// If Z,X coords modulus by 2 has rest, then round to number that has modulus from 2 zero
	//----------
	xCurr = fmod(x1, 2.0f);// x1 % 2;
	yCurr = fmod(y1, 2.0f);
	cout << "Params: " << params.x << ", " << params.y << ", " << params.z << endl;
	cout << "Coords: " << coords.x << ", " << coords.y << ", " << coords.z << endl;
	cout << "X1: " << x1 << endl;
	cout << "Y1: " << y1 << endl;


	xCurr = (x1 - xCurr);
	yCurr = (y1 - yCurr);

	cout << "X1Curr: " << xCurr << endl;
	cout << "Y1Curr: " << yCurr << endl;
	//----------

	GameWorld& g = GameWorld::getInstance();
	vec3 worldMin = g.getWorldOrigin();

	finish = false;
	counter = 0;
	while (!finish) {
		xCurr += 2;
		if (xCurr >= x2) {
			finish = true;
		}
		++counter;
		vec2 tileCoord = toTileCoords(vec3(xCurr-1, 0.0f, y1), worldMin);
		tiles.push_back(Tile(tileCoord));
		cout << "TileCoord: " << tileCoord.x << ", " << tileCoord.y << endl;
	}
	cout << "TilesCountX: " << counter << endl;
	finish = false;
	counter = 0;
	while (!finish) {
		yCurr += 2;
		if (yCurr >= y2) {
			finish = true;
		}
		++counter;
		vec2 tileCoord = toTileCoords(vec3(x1, 0.0f, yCurr-1), worldMin);
		tiles.push_back(Tile(tileCoord));
		cout << "TileCoord: " << tileCoord.x << ", " << tileCoord.y << endl;
	}
	cout << "TilesCountY: " << counter << endl;

	return tiles;
}