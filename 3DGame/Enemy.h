#pragma once
#ifndef ENEMY_H_
#define ENEMY_H_
#include "model.h"
#include "Unit.h"
#include "AnimatedModel.h"
using std::vector;

class Enemy : public Unit {
private:
	Enemy();
public:
	Enemy(string playerModelPath, int tileX, int tileY);
	Enemy(Model playerModel, int tileX, int tileY);
	Enemy(const Enemy& e);

	Enemy operator=(const Enemy& e);
};

#endif