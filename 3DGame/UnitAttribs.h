#pragma once
#ifndef  UNITATTRIBS_H_
#define UNITATTRIBS_H_

class UnitAttribs {
private:
	int life = 500;
	int maxArmor = 100;
	int currArmor = 50;

public:
	int getLife() {
		return life;
	}
	int getMaxArmor() { return maxArmor; }
	int getCurrArmor() { return currArmor; }

	//setters
	void setLife(int life) {
		this->life = life;
	}
	void setCurrArmor(int currArmor) {
		this->currArmor = currArmor;
	}
	void setMaxArmor(int maxArmor) {
		this->maxArmor = maxArmor;
	}

};

#endif

