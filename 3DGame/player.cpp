#include "player.h"
#include "Helper.h"
#include "GameWorld.h"
Player::Player() {}
Player::Player(const Player& p) {}
Player::Player(string modelPath, int tileX, int tileY) {
	GameWorld& g = GameWorld::getInstance();
	worldOriginX = g.getWorldOriginX()+1;
	worldOriginZ = g.getWorldOriginZ()+1;
	model.extractData(modelPath);
	goTo(tileX, tileY);
}
Player::Player(Model playerModel, int tileX, int tileY) {
	GameWorld& g = GameWorld::getInstance();
	worldOriginX = g.getWorldOriginX()+1;
	worldOriginZ = g.getWorldOriginZ()+1;
	model = playerModel;
	goTo(tileX, tileY);
}

//getters
int Player::getTileX() {
	return tileX;
}
int Player::getTileY() {
	return tileY;
}
Model Player::getModel() {
	return model;
} 
Tile Player::getTile() {
	return Tile(tileX,tileY);
}

//setters
void Player::setTileX(int x) {
	tileX = x;
	goTo(this->tileX, this->tileY);
}
void Player::setTileY(int y) {
	tileY = y;
	goTo(this->tileX, this->tileY);
}
void Player::setModel(Model model) {
	this->model = model;
}
void Player::setTile(Tile t) {
	this->tileX = t.getX();
	this->tileY = t.getY();
	goTo(this->tileX, this->tileY);
}
void Player::setRoute(vector<Tile> route, bool returnToStartPos) {
	this->route = route;
	currFrame = 0;
	totalFrames = (int)route.size();
	currFrameTime = 0.0f;
	frameTime = 500.0f/1000.0f;
	this->returnToStartPos = returnToStartPos;
	movementSide = 1;
}


//functions
void Player::translate(float x, float y, float z) {
	model.translate(x, y, z);
	vec3 coord = model.getMatrix()*vec4(1.0f);
	Helper h;
	vec2 tile = h.toTileCoords(coord, vec3(worldOriginX, 0,worldOriginZ));
	cout << "Player tile: " << tile.x << ", " << tile.y << endl;

}
void Player::goTo(int tileX, int tileY) {
	this->tileX = tileX;
	this->tileY = tileY;
	model.goTo(worldOriginX + tileY * 2.0f, 1.0f, worldOriginZ + tileX * 2.0f);
}
void Player::goTo(Tile t) {
	int tileX = t.getX();
	int tileY = t.getY();
	this->tileX = tileX;
	this->tileY = tileY;
	model.goTo(worldOriginX + tileY * 2.0f, 1.0f, worldOriginZ + tileX * 2.0f);
}
void Player::update(double time) {
	currFrameTime += time;
	if (currFrameTime >= frameTime) {//let's try to move
		if (movementSide == 1) {//start to end
			if (currFrame < totalFrames) {
				Tile currTile = route.at(currFrame);
				goTo(currTile);
				++currFrame;
			}
			else
				movementSide = 2;//change move direction
		}
		else if (movementSide == 2 && returnToStartPos) {//end to start
			if (currFrame > 0) {
				--currFrame;
				Tile currTile = route.at(currFrame);
				goTo(currTile);
			}
			else
				movementSide = 1;
		}
		currFrameTime -= frameTime;
	}
}
