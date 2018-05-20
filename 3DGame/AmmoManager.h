#pragma once
#ifndef AMMOMANAGER_H_
#define AMMOMANAGER_H_

#include "model.h"
#include "Ammo.h"
#include "Shader.h"
#include "AmmoProperty.h"
#include <list>
#include <map>
#include <glm/vec3.hpp>
using glm::vec3;
using std::map;
using std::pair;
using std::list;

class GameWorld;
class AmmoManager {
private:
	map<int, Model> models;//{type,model}
	map<int, AmmoProperty> properties;//{type,property}
	list<Ammo*> ammos;
public:
	AmmoManager(map<int, Model> models, map<int, AmmoProperty> properties);
	void addAmmo(int type, vec3 origin, vec3 direction);
	void addAmmo(Ammo* ammo);
	void addProperty(int type, AmmoProperty property);
	void addModel(int type, Model model);
	void clearAmmos();
	void update(float time);//miliseconds
	void draw(Shader shader);
	const list<Ammo*> AmmoManager::getAmmos();
	void removeAmmo(Ammo* ammo);
};

#endif