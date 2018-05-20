#pragma once
#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include <map>
#include "AmmoProperty.h"
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
	string AMMOS_FILEPATH;
	bool loaded = true;
	map<int, pair<string,string>> modelsPaths;
	map<int, AmmoProperty> ammosProperties;
	map<int, Model> ammosModels;
	Constants();
	Constants(const Constants& c);

	void parseAmmos(string path);
public:

	Constants::Constants(string gameSettingsFilepath, string modelsFilepath);

	//getters
	float worldOriginX();
	float worldOriginY();
	float worldOriginZ();
	int screenWidth();
	int screenHeight();
	map<int, pair<string, string>> Constants::getModelsPaths();
	map<int, AmmoProperty> Constants::getAmmosProperties();
	map<int, Model> Constants::getAmmosModels();
	bool isLoaded();

	//setters
	void screenWidth(int width);
	void screenHeight(int height);

};

#endif 
