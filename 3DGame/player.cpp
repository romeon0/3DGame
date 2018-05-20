#include "player.h"
#include "Helper.h"
#include "Tile.h"
#include "AnimatedModel.h"
#include "GameWorld.h"
#include "Unit.h"
Player::Player() {}
Player::Player(const Player& p) {}
Player::Player(string modelPath, int tileX, int tileY):Unit(1, modelPath, tileX, tileY) {
}
Player::Player(Model playerModel, int tileX, int tileY):Unit(1,playerModel,tileX,tileY) {
}
Player Player::operator=(const Player& e) {
	return Player(e.model, e.tileX, e.tileY);
}

