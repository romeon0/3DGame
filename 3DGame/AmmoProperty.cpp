#include "AmmoProperty.h"

AmmoProperty::AmmoProperty(int type, string name, string desc, 
	float damage, float speed, float penetration) {
	this->type = type;
	this->name = name;
	this->desc = desc;
	this->damage = damage;
	this->speed = speed;
	this->penetration = penetration;
}
int AmmoProperty::getType() {
	return type;
}
float AmmoProperty::getDamage() {
	return damage;
}
float AmmoProperty::getSpeed() {
	return speed;
}
float AmmoProperty::getPenetration() {
	return penetration;
}