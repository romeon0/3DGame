//made by Romeon0
#include <glm/vec3.hpp>
#include <iostream>
#include "AlgRayBoxAABB.h"
using glm::vec3;

bool AlgRayBoxAABB::isCollided(vec3 boxMin, vec3 boxMax, vec3 rayOrigin, vec3 rayDir) {

	vec3 t1, t2; // vectors to hold the T-values for every direction
	float tNear = -FLT_MAX;
	float tFar = FLT_MAX;

	if (rayDir.x == 0) { // ray parallel to planes in this direction
		if ((rayOrigin.x < boxMin.x) || (rayOrigin.x > boxMax.x)) {
			return false; // parallel AND outside box : no intersection possible
		}
	}
	else {
		t1.x = (boxMin.x - rayOrigin.x) / rayDir.x;
		t2.x = (boxMax.x - rayOrigin.x) / rayDir.x;

		if (t1.x > t2.x) { // we want t1 to hold values for intersection with near plane
			std::swap(t1, t2);
		}
		if (t1.x > tNear) {
			tNear = t1.x;
		}
		if (t2.x < tFar) {
			tFar = t2.x;
		}
		if ((tNear > tFar) || (tFar < 0)) {
			return false;
		}
	}

	if (rayDir.y == 0) { // ray parallel to planes in this direction
		if ((rayOrigin.y < boxMin.y) || (rayOrigin.y > boxMax.y)) {
			return false; // parallel AND outside box : no intersection possible
		}
	}
	else {
		t1.y = (boxMin.y - rayOrigin.y) / rayDir.y;
		t2.y = (boxMax.y - rayOrigin.y) / rayDir.y;

		if (t1.y > t2.y) { // we want t1 to hold values for intersection with near plane
			std::swap(t1, t2);
		}
		if (t1.y > tNear) {
			tNear = t1.y;
		}
		if (t2.y < tFar) {
			tFar = t2.y;
		}
		if ((tNear > tFar) || (tFar < 0)) {
			return false;
		}
	}

	if (rayDir.z == 0) { // ray parallel to planes in this direction
		if ((rayOrigin.z < boxMin.z) || (rayOrigin.z > boxMax.z)) {
			return false; // parallel AND outside box : no intersection possible
		}
	}
	else {
		t1.z = (boxMin.z - rayOrigin.z) / rayDir.z;
		t2.z = (boxMax.z - rayOrigin.z) / rayDir.z;

		if (t1.z > t2.z) { // we want t1 to hold values for intersection with near plane
			std::swap(t1, t2);
		}
		if (t1.x > tNear) {
			tNear = t1.z;
		}
		if (t2.z < tFar) {
			tFar = t2.z;
		}
		if ((tNear > tFar) || (tFar < 0)) {
			return false;
		}
	}
	std::cout << "tNear: " << tNear << std::endl;
	std::cout << "tFar: " << tFar << std::endl;
	return true; // if we made it here, there was an intersection - YAY
}

vec3 AlgRayBoxAABB::getTNear() {
	return tnear;
}
vec3 AlgRayBoxAABB::getTFar() {
	return tfar;
}
