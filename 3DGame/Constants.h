#pragma once
#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include <map>
#include <string>
using std::string;
using std::map;
using std::pair;

class Constants {
private:
	float WORLDORIGIN_X;
	float WORLDORIGIN_Y;
	float WORLDORIGIN_Z;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	bool loaded = true;
	map<int, pair<string,string>> modelsPaths;
	Constants();
	Constants(const Constants& c);
public:

	Constants::Constants(string gameSettingsFilepath, string modelsFilepath);

	//getters
	float worldOriginX();
	float worldOriginY();
	float worldOriginZ();
	int screenWidth();
	int screenHeight();
	map<int, pair<string, string>> Constants::getModelsPaths();
	bool isLoaded();

	//setters
	void screenWidth(int width);
	void screenHeight(int height);

};

#endif 
