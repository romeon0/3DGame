#pragma once
#ifndef AMMO_H_
#define AMMO_H_

#include "AmmoProperty.h"
#include "model.h"
#include <glm/vec3.hpp>
using glm::vec3;

class AmmoProperty;
class Ammo {
private:
	//information
	int type=0;//unique 
	int count = 0;
	AmmoProperty* ammoProperty;

	//movement
	vec3 currentPosition=vec3(0.0f,5.0f,0.0f);
	vec3 distance, speed, origin;
	float time=0.0f;
	float currentTime=0.0f;
	float radius = 100.0f;
	int status;
	//drawing
	Model model;//flying model

public:
	int tmp;

	Ammo(Model ammoModel, AmmoProperty* ammoProperty, vec3 origin, vec3 direction);

	//getters
	Model getModel();
	vec3 getSpeed();
	vec3 getOrigin();
	 AmmoProperty getProperty();
	int getStatus();//1-alive,2-radius flied

	//setters
	void setModel(Model model);

	//functions
	void update(double time);
	int getType();
};

#endif
