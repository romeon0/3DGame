#include "Helper.h"
#include "Tile.h"
#include <glm/vec3.hpp>
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
	vec3 offset = terrainCoords - terrainMin;
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