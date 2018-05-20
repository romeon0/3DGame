#pragma once
#ifndef AMMOPROPERTY_H_
#define AMMOPROPERTY_H_

#include "model.h"
#include <vector>
#include <iostream>
#include <map>
#include <glm/vec3.hpp>
using glm::vec3;
using std::map;
using std::vector;

class Ammo;
class AmmoProperty {
private:
	int type;//unique
	float damage;
	float speed;
	float penetration;
	string name;
	string desc;


public:
	AmmoProperty() {}
	AmmoProperty(int type, string name, string desc, float damage, float speed, float penetration);
/*	Ammo(const Ammo& a) {
	std::cout << "Hello there2!===============================\n";
	}*/
	
	int getType();
	float getDamage();
	float getSpeed();
	float getPenetration();
};

#endif