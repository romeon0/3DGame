#pragma once
#ifndef _ALGRAYBOXABB_H_
#define  _ALGRAYBOXABB_H_

#include <glm/vec3.hpp>
using glm::vec3;

class AlgRayBoxAABB {
private:
	vec3 tnear, tfar;
public:
	bool isCollided(vec3 boxMin, vec3 boxMax, vec3 rayOrigin, vec3 rayDir);
	vec3 getTNear();
	vec3 getTFar();
};

#endif