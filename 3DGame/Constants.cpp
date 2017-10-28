#include "Constants.h"
#include "GameWorld.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
using std::string;
using std::vector;
using std::ifstream;
using std::cout;
using std::endl;
using std::pair;

//davos.science.upm.ro/~traian/ web_curs/index_ing.html
//student@upm.ro studentupm
//xampp, 
Constants::Constants() {}
Constants::Constants(const Constants& c) {}
	

Constants::Constants(string gameSettingsFilepath, string modelsFilepath)
{
	//TODO implement file format exceptions
	ifstream file;
	string line;
	string attribName;
	string tmp;
	float floatAttribValue;
	string stringAttribValue;
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
		std::getline(file, line);
		ss << line;
		ss >> attribName;
		ss >> tmp;
		ss >> floatAttribValue;
		if (attribName.compare("ScreenWidth") == 0) { SCREEN_WIDTH = floatAttribValue; }
		else if (attribName.compare("ScreenHeight") == 0) { SCREEN_HEIGHT = floatAttribValue; }
		else {
			cout << "Settings: Error in parsing '" << gameSettingsFilepath << "'.\n";
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
		cout << "Line1: " << ss.str() << endl;
		ss >> attribName;//id
		ss >> tmp;// = 
		ss >> id;//3445
		ss.str(string());
		ss.clear();//clear buffer
		std::getline(file, line);//Path = 3DModel2/tile.obj
		ss << line;
		cout << "Line2: " << ss.str() << endl;
		ss >> attribName;//Path
		ss >> tmp;//=
		ss >> path;//	data/models/.../model.obj
		ss.str(string());
		ss.clear();//clear buffer
		std::getline(file, line);//Name = Player
		ss << line;
		cout << "Line3: " << ss.str() << endl;
		ss >> attribName;//Name
		ss >> tmp;//=
		ss >> name;//terrain
		std::getline(file, line);//remove space
		ss.str(string());
		ss.clear();//clear buffer

		cout << "INP: " <<id << ":" << name <<", " << path << endl;
		pair<string, string> pathAndName;
		pathAndName.first = name;
		pathAndName.second = path;
		pair_.first = id;
		pair_.second = pathAndName;
		modelsPaths.insert(pair_);
	}
	file.close();
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

//setters
void Constants::screenWidth(int width) { SCREEN_WIDTH = width; }
void Constants::screenHeight(int height) { SCREEN_HEIGHT = height; }


