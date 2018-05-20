#include "Constants.h"
#include "GameWorld.h"
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
using std::string;
using std::vector;
using std::map;
using std::ifstream;
using std::cout;
using std::endl;
using std::pair;

Constants::Constants() {}
Constants::Constants(const Constants& c) {}
	

Constants::Constants(string gameSettingsFilepath, string modelsFilepath)
{
	//TODO implement file format exceptions
	ifstream file;
	string line;
	string attribName;
	string tmp;
	int intValue;
	string strValue;
	std::stringstream ss;
	GameWorld& g = GameWorld::getInstance();

	//game.stg
	file.open(gameSettingsFilepath);
	if (!file.is_open()) {
		cout << "Can't open '" << gameSettingsFilepath <<"' file." << endl;
		loaded = false;
		return;
	}
	while (!file.eof()) {
		file >> attribName;
		if (attribName.length() == 0) continue;
		file >> tmp;
		if (attribName.length() == 0) continue;
		file >> strValue;
		if (attribName.length() == 0) continue;

		if (attribName.compare("ScreenWidth") == 0) {
			ss << strValue;
			ss >> intValue;
			SCREEN_WIDTH = intValue;
		}
		else if (attribName.compare("ScreenHeight") == 0) {
			ss << strValue;
			ss >> intValue;
			SCREEN_HEIGHT = intValue;
		}
		else if (attribName.compare("AmmosPath") == 0) {
			AMMOS_FILEPATH = strValue;
			
		}
		else {
			cout << "Settings: Error in parsing '" << gameSettingsFilepath << "'. ";
			cout << "Wrong attribute '" << attribName << "'.\n";
			loaded = false;
			system("pause");
			break;
		}
		ss.clear();
	}
	file.close();
	ss.str("");

	//models.stg
	int id;
	string name, path;
	file.open(modelsFilepath);
	if (!file.is_open()) {
		cout << "Can't open '" << modelsFilepath << "' file." << endl;
		loaded = false;
		return;
	}
	std::pair<int, pair<string,string>> pair_;
	while (!file.eof()) {
		std::getline(file, line);//remove [Model]
		if (line.compare("") == 0) break;
		std::getline(file, line);//id = 3445
		if (line.compare("") == 0) break;

		ss << line;
		ss >> attribName;//id
		ss >> tmp;// = 
		ss >> id;//3445
		ss.str(string());
		ss.clear();//clear buffer
		std::getline(file, line);//Path = 3DModel2/tile.obj
		ss << line;
		ss >> attribName;//Path
		ss >> tmp;//=
		ss >> path;//	data/models/.../model.obj
		ss.str(string());
		ss.clear();//clear buffer
		std::getline(file, line);//Name = Player
		ss << line;
		ss >> attribName;//Name
		ss >> tmp;//=
		ss >> name;//terrain
		std::getline(file, line);//remove space
		ss.str(string());
		ss.clear();//clear buffer

		pair<string, string> pathAndName;
		pathAndName.first = name;
		pathAndName.second = path;
		pair_.first = id;
		pair_.second = pathAndName;
		modelsPaths.insert(pair_);
	}
	file.close();

	//ammo.lst
	parseAmmos(AMMOS_FILEPATH);

}

//getters
float Constants::worldOriginX() { return WORLDORIGIN_X; }
float Constants::worldOriginY() { return WORLDORIGIN_Y; }
float Constants::worldOriginZ() { return WORLDORIGIN_Z; }
int Constants::screenWidth() { return SCREEN_WIDTH; }
int Constants::screenHeight() { return SCREEN_HEIGHT; }
map<int, pair<string,string>> Constants::getModelsPaths() {
	return modelsPaths;
}
bool Constants::isLoaded() {
	return loaded;
}
map<int,AmmoProperty> Constants::getAmmosProperties() {
	return ammosProperties;
}
map<int, Model> Constants::getAmmosModels() {
	return ammosModels;
}
//setters
void Constants::screenWidth(int width) { SCREEN_WIDTH = width; }
void Constants::screenHeight(int height) { SCREEN_HEIGHT = height; }

void Constants::parseAmmos(string filePath) {

}
