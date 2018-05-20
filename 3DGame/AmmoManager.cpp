#include "AmmoManager.h"

AmmoManager::AmmoManager(map<int, Model> models, map<int, AmmoProperty> properties) {
	this->properties = properties;
	this->models = models;
}

void AmmoManager::addAmmo(Ammo* ammo) {
	ammos.push_back(ammo);
}
void AmmoManager::addAmmo(int type, vec3 origin, vec3 direction) {
	if (models.find(type) == models.end()) return;
	if (properties.find(type) == properties.end()) return;

	Ammo* ammo = new Ammo(models[type], &properties[type], origin, direction);
	ammos.push_back(ammo);
}
void AmmoManager::addProperty(int type, AmmoProperty prop) {
	properties.insert(properties.end(),pair<int, AmmoProperty>(type, prop));
}
void AmmoManager::addModel(int type, Model model) {
	models.insert(models.end(), pair<int, Model>(type, model));
}
//
void AmmoManager::clearAmmos() {
	ammos.clear();
}
void AmmoManager::update(float time) {
	vector<Ammo*> toDelete;
	//find ammos out of radius
	for (Ammo* ammo : ammos) {
		if (ammo->getStatus() == 2)
			toDelete.push_back(ammo);
	}
	//remove ammos out of radius
	for (Ammo* ammo : toDelete) {
		ammos.remove(ammo);
	}
	//update positions
	for (Ammo* ammo : ammos) {
		ammo->update(time);
	}
}
void AmmoManager::draw(Shader shader) {
	for (Ammo* ammo : ammos) {
		Model model = ammo->getModel();
		shader.setMat4("model", model.getMatrix());
		model.draw(shader);
	}
}

const list<Ammo*> AmmoManager::getAmmos() {
	return ammos;
}

void AmmoManager::removeAmmo(Ammo* ammo) {
	ammos.remove(ammo);
}