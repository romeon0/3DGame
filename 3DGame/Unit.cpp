#include "Unit.h"
#include "Helper.h"
#include "Tile.h"
#include "Model.h"
#include "GameWorld.h"

//constructors
Unit::Unit() {}
Unit::Unit(const Unit& u) {}
Unit::Unit(int type, string modelPath, int tileX, int tileY) {
	this->type = type;
	GameWorld& g = GameWorld::getInstance();
	worldOriginX = g.getWorldOriginX();
	worldOriginY = g.getWorldOriginY();
	worldOriginZ = g.getWorldOriginZ();
	model.extractData(modelPath);
	goToTile(tileX, tileY);
	name = "ENEMY";
}
Unit::Unit(int type, Model playerModel, int tileX, int tileY) {
	this->type = type;
	GameWorld& g = GameWorld::getInstance();
	worldOriginX = g.getWorldOriginX();
	worldOriginY = g.getWorldOriginY();
	worldOriginZ = g.getWorldOriginZ();
	model = playerModel;
	goToTile(tileX, tileY);
	name = "ENEMY";
}

//getters
int Unit::getTileX() {
	return tileX;
}
int Unit::getTileY() {
	return tileY;
}
Model& Unit::getModel() {
	return model;
}
Tile Unit::getTile() {
	return Tile(tileX, tileY);
}
string Unit::getName() {
	return name;
}

//setters
void Unit::setTileX(int x) {
	tileX = x;
	goToTile(this->tileX, this->tileY);
}
void Unit::setTileY(int y) {
	tileY = y;
	goToTile(this->tileX, this->tileY);
}
void Unit::setModel(Model model) {
	this->model = model;
}
void Unit::setTile(Tile t) {
	this->tileX = t.getX();
	this->tileY = t.getY();
	goToTile(this->tileX, this->tileY);
}
void Unit::setRoute(vector<Tile> route, bool returnToStartPos) {
	this->route = route;
	currFrame = 0;
	totalFrames = (int)route.size();
	currFrameTime = 0.0f;
	frameTime = 500.0f;//miliseconds
	this->returnToStartPos = returnToStartPos;
	movementSide = 1;
}

void Unit::setName(string name)
{
	this->name = name;
}


//functions
void Unit::translate(float x, float y, float z) {
	model.translate(x, y, z);
	vec3 coord = model.getCoords();
	Helper h;
	vec2 tile = h.toTileCoords(coord, vec3(worldOriginX, worldOriginY, worldOriginZ));
	cout << "Player tile: " << tile.x << ", " << tile.y << endl;
	tileX = tile.x;
	tileY = tile.y;
}
void Unit::translate(Tile t) {
	model.translate(t.getX(), worldOriginY, t.getY());
	this->tileX = tileX;
	this->tileY = tileY;
	vec3 coord = model.getCoords();
	Helper h;
	vec2 tile = h.toTileCoords(coord, vec3(worldOriginX, 0, worldOriginZ));
	cout << "Player tile: " << tile.x << ", " << tile.y << endl;

}
void Unit::goToTile(int tileX, int tileY) {
	this->tileX = tileX;
	this->tileY = tileY;
	model.goTo(worldOriginX+tileY * 2.0f, 0, worldOriginZ+tileX * 2.0f);
}
void Unit::goToTile(Tile t) {
	int tileX = t.getX();
	int tileY = t.getY();
	this->tileX = tileX;
	this->tileY = tileY;
	model.goTo(tileY * 2.0f-1, 0, tileX * 2.0f-1);
}

void Unit::move(double time) {
	currFrameTime += time;
	if (currFrameTime >= frameTime) {//let's try to move
		currFrameTime -= frameTime;

		if (movementSide == 1) {//running from start position to end position
			if (currFrame < totalFrames) {
				Tile currTile = route.at(currFrame);
				float degree = calculateRotationDegree(currTile);
				goToTile(currTile);
				rotate(degree);
				++currFrame;
			}
			else
				movementSide = 2;//change move direction
		}
		else if (movementSide == 2) { //end to start
			if (!returnToStartPos) { route.clear(); }//if we don't need to return to start position then route finished
			else {
				if (currFrame > 0) {
					--currFrame;
					Tile currTile = route.at(currFrame);
					goToTile(currTile);
				}
				else
					route.clear();//route finished
			}
		}
	}
}

void Unit::update(double time) {
	if (route.size() != 0) {//if player has route
		move(time);
	}
}

bool Unit::hasRoute() {
	return route.size() == 0 ? false : true;
}

float Unit::calculateRotationDegree(Tile nextTile) {
	int deltaX = tileX - nextTile.getX();
	int deltaY = tileY - nextTile.getY();

	if (deltaX == 1 && deltaY == 1) return -135.0f;//left up corner
	if (deltaX == 0 && deltaY == 1) return -90.0f;//up
	if (deltaX == -1 && deltaY == 1) return -45.0f;//right up corner
	if (deltaX == 1 && deltaY == 0) return 180.0f;//left
	if (deltaX == 0 && deltaY == 0) return 0.0f;//the same coord
	if (deltaX == -1 && deltaY == 0) return 0.0f;//right
	if (deltaX == 1 && deltaY == -1) return 135.0f;//left bottom corner 
	if (deltaX == 0 && deltaY == -1) return 90.0f;//bottom
	if (deltaX == -1 && deltaY == -1) return 45.0f;//right bottom corner


	return 0.0f;
}

void Unit::rotate(float degree) {
	model.rotateDegree(degree, 2);
}

