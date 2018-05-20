#include <iostream>
#include "Ammo.h"
#include "Helper.h"


Ammo::Ammo(Model ammoModel, AmmoProperty* ammoProperty, vec3 origin, vec3 direction) {
	distance = direction;
	time = length(distance);
	speed = (distance / time);
	speed *= 0.01;

	currentPosition = origin;
	model.setCoords(currentPosition);
	currentTime = 0.0f;
	this->origin = origin;
	this->ammoProperty = ammoProperty;
	model = ammoModel;
	status = 1;//alive
}

//getters
Model Ammo::getModel() {
	return model;
}
vec3 Ammo::getSpeed() {
	return speed;
}
vec3 Ammo::getOrigin() {
	return origin;
}
int Ammo::getType() {
	return type;
}
int Ammo::getStatus() { return status; }

//setters
void Ammo::setModel(Model model) {}

//functions
void Ammo::update(double time) {//in milliseconds
	float currPosLen = length(currentPosition - origin);
	if (currPosLen >= radius) {
		status = 2;
		return;
	}

//	time /= 100.0f;
	currentTime += time;


	currentPosition = origin+speed*currentTime;
	model.setCoords(currentPosition);

	//tmp
	/*Helper h;
	cout << "-----START-----"<< endl;
	cout << "Time: " << time << endl;
	cout << "currentTime: " << currentTime << endl;
	cout << "origin: \n";
	h.showVector(origin);
	cout << "speed: \n";
	h.showVector(speed);
	cout << "currentTime*speed: \n";
	h.showVector(currentTime*speed);
	cout << "currentPosition: \n";
	h.showVector(currentPosition);
	cout << "-----END-----" << endl;*/
}
AmmoProperty Ammo::getProperty() {
	return *ammoProperty;
}