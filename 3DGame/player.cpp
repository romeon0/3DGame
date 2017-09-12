#include "player.h"
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

//setters
void Player::setTileX(int x) {
	tileX = x;
}
void Player::setTileY(int y) {
	tileY = y;
}
void Player::setModel(Model model) {
	this->model = model;
}