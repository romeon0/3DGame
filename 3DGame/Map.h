#pragma once
#ifndef MAP_H_
#define MAP_H_
#include "Tile.h"
#include <vector>
#include "model.h"
#include <glm/mat4x4.hpp>
using std::vector;

class Map {
private:
	Model terrain;
	vector<Model> objects;
	vector<Tile> tiles;
	int width, height;
	int** gridPatency;
public:
	Map(int width, int height);

	//getters
	vector<Tile> getNeighbors(Tile t);
	int getWidth();
	int getHeight();
	mat4 getMatrix(string objName);
	vector<Model> getModels();
	vec3 getTerrainVolume();
	vec3 getTerrainCoords();
	int** getGridPatency();

	//other
	void addObject(string modelPath, string modelName);
	void addObject(Model model);
	void draw(Shader shader);
	bool isBlocked(int x, int y);

};

#endif MAP_H_