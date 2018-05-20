#include "Enemy.h"
#include "Model.h"
#include "Unit.h"
Enemy::Enemy() {}
Enemy::Enemy(const Enemy& e){

	this->route=route;//current unit path
	this->currFrame = e.currFrame;
	this->totalFrames = e.totalFrames;
	this->currFrameTime = e.currFrameTime;
	this->frameTime = e.frameTime;
	this->returnToStartPos= e.returnToStartPos;
	this->movementSide = e.movementSide;

	this->tileX = e.tileX;
	this->tileY = e.tileY;
	this->worldOriginX = e.worldOriginX;
	this->worldOriginY = e.worldOriginY;
	this->worldOriginZ = e.worldOriginZ;
	this->model= e.model;
	this->type= e.type;
}
Enemy::Enemy(string modelPath, int tileX, int tileY):Unit(2,modelPath,tileX,tileY) {
}
Enemy::Enemy(Model playerModel, int tileX, int tileY):Unit(2, playerModel, tileX, tileY) {
}

Enemy Enemy::operator=(const Enemy& e) {
	return Enemy(e.model, e.tileX, e.tileY);
}

