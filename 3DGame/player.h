#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include "model.h"
#include "Tile.h"
#include <vector>
using std::vector;

class Player {
private:
	//movement
	vector<Tile> route;
	int currFrame, totalFrames;
	double currFrameTime, frameTime;
	bool returnToStartPos;
	int movementSide;

	//coords
	int tileX, tileY;
	int worldOriginX, worldOriginZ;
	Model model;

	Player();
	Player(const Player& p);

	void goTo(int tileX, int tileY);
	void goTo(Tile t);
public:
	Player(string playerModelPath, int tileX, int tileY);
	Player(Model playerModel, int tileX, int tileY);
	//getters
	int Player::getTileX();
	int Player::getTileY();
	Tile getTile();
	Model getModel();

	//setters
	void Player::setTileX(int x);
	void Player::setTileY(int y);
	void Player::setTile(Tile t);
	void setModel(Model model);
	void setRoute(vector<Tile> route, bool returnToStartPos);

	//functions
	void translate(float x, float y, float z);
	void update(double time);
};

#endif