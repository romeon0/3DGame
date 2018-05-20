#pragma once
#ifndef GAMEWORLD_H_
#define GAMEWORLD_H_

#define GL_GLEXT_PROTOTYPES
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cfloat>
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "AnimatedModel.h"
#include "SimpleModel.h"
#include "AlgRayBoxAABB.h"
#include "AlgRayCasting.h"
#include "Helper.h"
#include "player.h"
#include "Enemy.h"
#include "AlgAStarPathfinder.h"
#include "Map.h"
#include "AmmoManager.h"
#include "TextManager.h"
#include "ProgressBar.h"
using glm::vec3;
using glm::mat4;



class GameWorld {
private:
	// settings
	unsigned int SCR_WIDTH = 800;
	unsigned int SCR_HEIGHT = 600;
	float worldOriginX, worldOriginY, worldOriginZ;
	// camera
	Camera* camera;
	double lastX = SCR_WIDTH / 2.0;
	double lastY = SCR_HEIGHT / 2.0;
	bool firstMouse = true;
	// timing
	float currentDeltaTime;
	long long currentTime;
	long long lastTime;
	float FPS = 30.0f;// frames per second
	float FRAME_TIME = (1000.0f / FPS);//miliseconds
	//models/objects
	SimpleModel* clickedLine;
	SimpleModel* pathLine;
	SimpleModel* screenBox;
	SimpleModel* mouseModel;
	SimpleModel* centerLine;
	//objects settings
	bool drawGrid = true;
	//matrices
	mat4 projection, view;//world matrices
	mat4 clickedMatrix;//
	//window
	GLFWwindow* window;
	//shaders
	Shader *ourShader, *gridShader, *lineShader, *primitiveScreenShader,*animShader;
	Shader* outlineShader, *colorShader, *mouseShader;
	//other classes
	AlgRayCasting* rayCasting;
	AlgRayBoxAABB* rayboxCollision;
	Helper helper;
	AlgAStarPathfinder* pathfinder;
	//map
	Map* gameMap;
	AmmoManager* ammoManager;
	//Player
	Player* player;
	Model* model;
	//
	Enemy* selectedEnemy;
	TextManager* textManager;

	// process all inputs
	//-------------------
	//-realtime process input(not using)
	void processInput(GLFWwindow *window);
	//-process keyboard
	static void  keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void framebuffer_sizeCallback(GLFWwindow* window, int width, int height);
	static void mouseCursorCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	//-------------------

	int init();
	void finish();
	int run();
	void draw();
	void collide();
	void drawWithOutline(AnimatedModel* model);
	void drawWithOutline(Model* model, vec3 color=vec3(0.0f, 0.0f, 0.0f));
	void drawMouse();
	void drawLines(vector<Vertex> vertices, vector<unsigned int> indices);
	void update(double time);
	void input();
	void drawSimpleCube(vec3 min, vec3 max);

	//ray colliding
	map<double, Enemy*> checkRayWithEnemies(vec3 origin, vec3 direction);
	map<double, Model*>checkRayWithMapObjs(vec3 origin, vec3 direction);
	pair<bool, double> checkRayWithPlayer(vec3 origin, vec3 direction);
	bool checkRayWithTerrain(vec3 origin, vec3 direction);
	//body colliding
	bool checkCollide(vec3 coordsBody1, vec3 volumeBody1, vec3 coordsBody2, vec3 volumeBody2);

	GameWorld();
	GameWorld::GameWorld(const GameWorld& g);
	GameWorld operator=(const GameWorld& g) {
		return *this;
	}
public:
	vector<Enemy*> enemies;
	SimpleModel* simpleCube;
	SimpleModel* enemyInfo;
	SimpleModel* mainPanel;
	SimpleModel* playerInfo;
	ProgressBar* bar;
	AnimatedModel* animModel;

	void start();

	static GameWorld &getInstance() {
		static GameWorld instance;
		return instance;
	}

	float getWorldOriginX() {
		return worldOriginX;
	}
	float getWorldOriginY() {
		return worldOriginY;
	}
	float getWorldOriginZ() {
		return worldOriginZ;
	}

	vec3 getWorldOrigin() {
		return vec3(worldOriginX, worldOriginY, worldOriginZ);
	}

	

	//testing
	void GameWorld::enemyAttacked(Enemy* enemy);
	void GameWorld::terrainAttacked(vec3 origin, vec3 direction);
	void drawText(string text, double screenX, double screenY);
	void initGUIImage(SimpleModel & model, double screenX, double screenY, double width, double height);
};

#endif GAMEWORLD_H_