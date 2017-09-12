#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "player.h"
#include "model.h"
class Player {
private:
	int tileX, tileY;
	Model model;
public:
	//getters
	int Player::getTileX();
	int Player::getTileY();
	Model getModel();

	//setters
	void Player::setTileX(int x);
	void Player::setTileY(int y);
	void setModel(Model model);
};

#endif