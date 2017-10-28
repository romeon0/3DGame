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
#include "SimpleModel.h"
#include "AlgRayBoxAABB.h"
#include "AlgRayCasting.h"
#include "Helper.h"
#include "player.h"
#include "AlgAStarPathfinder.h"
#include "Map.h"
using glm::vec3;
using glm::mat4;

class GameWorld {
private:
	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	float worldOriginX, worldOriginY, worldOriginZ;
	// camera
	Camera* camera;
	double lastX = SCR_WIDTH / 2.0;
	double lastY = SCR_HEIGHT / 2.0;
	bool firstMouse = true;
	// timing
	float currDeltaTime = 0.0f;
	float totalDeltaTime = 0.0f;
	float lastFrame = 0.0f;
	//models/objects
	SimpleModel* clickedLine;
	SimpleModel* pathLine;
	SimpleModel* screenBox;
	//objects settings
	bool drawGrid = true;
	//matrices
	mat4 projection, view;//world matrices
	mat4 clickedMatrix;//
	//window
	GLFWwindow* window;
	//shaders
	Shader *ourShader, *gridShader, *lineShader, *screenShader;
	//other classes
	AlgRayCasting* rayCasting;
	AlgRayBoxAABB* rayboxCollision;
	Helper helper;
	AlgAStarPathfinder* pathfinder;
	//map
	Map* gameMap;
	//Player
	Player* player;

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
	void update(double time);
	void input();

	GameWorld();
	GameWorld::GameWorld(const GameWorld& g);
	GameWorld operator=(const GameWorld& g) {
		return *this;
	}
public:
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
};

#endif GAMEWORLD_H_