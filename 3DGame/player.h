#pragma once
#ifndef PLAYER_H_
#define PLAYER_H_
#include "model.h"
#include "Tile.h"
#include "Unit.h"
#include "AnimatedModel.h"
#include <vector>
using std::vector;

class Player : public Unit{
private:
	Player();
	Player(const Player& p);
public:
	Player(string playerModelPath, int tileX, int tileY);
	Player(Model playerModel, int tileX, int tileY);
	Player Player::operator=(const Player& e);
};

#endif