#include<vector>
#include "Model.h"
#include "Tile.h"
#include "UnitAttribs.h"

#pragma once
#ifndef UNIT_H_
#define UNIT_H_

class Unit {
protected:
	string name = "ENEMY";
	//movement
	vector<Tile> route;//current unit path
	int currFrame, totalFrames;
	double currFrameTime, frameTime;
	bool returnToStartPos;
	int movementSide;
	//unit attributes
	int currLife = 300;
	int maxLife = 500;
	int maxArmor = 100;
	int currArmor = 50;

	int tileX, tileY;//current coords
	float worldOriginX, worldOriginY,worldOriginZ;//world origins
	Model model;//unit model
	int type;

	Unit();
	Unit(const Unit& u);

	void goToTile(int tileX, int tileY);
	void goToTile(Tile t);
	void move(double time);
	float calculateRotationDegree(Tile nextTile);

public:
	Unit(int type, string playerModelPath, int tileX, int tileY);
	Unit(int type, Model playerModel, int tileX, int tileY);

	//getters
	int getTileX();
	int getTileY();
	Tile getTile();
	Model& getModel();
	string getName();

	//setters
	void setTileX(int x);
	void setTileY(int y);
	void setTile(Tile t);
	void setModel(Model model);
	void setRoute(vector<Tile> route, bool returnToStartPos);
	void setName(string name);

	//functions
	void translate(float x, float y, float z);
	void translate(Tile tile);
	void update(double time);
	bool hasRoute();
	void rotate(float degree);

	//mechanics
	int getMaxLife() {
		return maxLife;
	}
	int getCurrLife() { return currLife; }
	int getMaxArmor() { return maxArmor; }
	int getCurrArmor() { return currArmor; }

	//setters
	void setMaxLife(int life) {
		this->maxLife = life;
	}
	void setCurrLife(int life) {
		this->currLife = life;
	}
	void setCurrArmor(int currArmor) {
		this->currArmor = currArmor;
	}
	void setMaxArmor(int maxArmor) {
		this->maxArmor = maxArmor;
	}
	void damage(int dmg) {
		setCurrLife(getCurrLife() - dmg);
	}
};

#endif