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
#include <fstream>
#include <random>
#include <vector>
#include <sstream>
#include <cfloat>
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "SimpleModel.h"
#include "AlgRayBoxAABB.h"
#include "AlgRayCasting.h"
#include "GameWorld.h"
#include "Constants.h"
using glm::vec3;
using glm::mat4;


vector<Vertex> vts;
vector<uint> ids;
Vertex v1, v2, v3, v4;

int** matrix;
//constructor
GameWorld::GameWorld() {
	
}

GameWorld::GameWorld(const GameWorld& g) {}

// process all inputs
//-------------------
//-realtime process input(not using)
void GameWorld::processInput(GLFWwindow *window)
{
	GameWorld& g = GameWorld::getInstance();
	float deltaTime = g.currDeltaTime;
	Player* player = g.player;

	float velocity = 0.125;
	vec3 up(0.0f, 1.0f, 0.0f);
	vec3 down(0.0f, -1.0f, 0.0f);
	vec3 right(1.0f, 0.0f, 0.0f);
	vec3 left(-1.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		g.camera->processKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		g.camera->processKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		g.camera->processKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		g.camera->processKeyboard(RIGHT, deltaTime);
	}
	float pas = 2.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		g.player->translate(0.0, 0.0, -pas);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		g.player->translate(0.0, 0.0, pas);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		g.player->translate(pas, 0, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		g.player->translate(-pas, 0, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		g.camera->translate(0, 0.5f, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		g.camera->translate(0, -0.5f, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		g.drawGrid = !g.drawGrid;
	}
	vec3 pos = g.camera->getPosition();
	std::cout << "Camera position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
}
//-process keyboard
void GameWorld::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GameWorld& g = GameWorld::getInstance();
	float deltaTime = g.currDeltaTime;
	Player* player = g.player;

	float velocity = 0.125;
	vec3 up(0.0f, 1.0f, 0.0f);
	vec3 down(0.0f, -1.0f, 0.0f);
	vec3 right(1.0f, 0.0f, 0.0f);
	vec3 left(-1.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true); 

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		g.camera->processKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		g.camera->processKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		g.camera->processKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		g.camera->processKeyboard(RIGHT, deltaTime);
	}
	float pas = 2.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		g.player->translate(0.0, 0.0, -pas);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		g.player->translate(0.0, 0.0, pas);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		g.player->translate(pas, 0, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		g.player->translate(-pas, 0, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		g.camera->translate(0, 0.5f, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		g.camera->translate(0, -0.5f, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		g.drawGrid = !g.drawGrid;
	}
	vec3 pos = g.camera->getPosition();
	std::cout << "Camera position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
}
void GameWorld::framebuffer_sizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
void  GameWorld::mouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (window == NULL)
		cout << "Null mouseCursor window!\n";

	GameWorld& g = GameWorld::getInstance();
	if (g.firstMouse)
	{
		g.lastX = xpos;
		g.lastY = ypos;
		g.firstMouse = false;
	}

	double xoffset = xpos - g.lastX;
	double yoffset = g.lastY - ypos; // reversed since y-coordinates go from bottom to top

	g.lastX = xpos;
	g.lastY = ypos;
	g.camera->processMouseMovement((float)xoffset, (float)yoffset);

}
void GameWorld::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	GameWorld& g = GameWorld::getInstance();
	g.camera->processMouseScroll((float)yoffset);
}
void GameWorld::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		GameWorld& g = GameWorld::getInstance();
		double viewportX, viewportY;
		glfwGetCursorPos(window, &viewportX, &viewportY);

		//calculate vector origin & direction
		g.rayCasting->calculate((float)viewportX, (float)viewportY, (float)g.SCR_WIDTH, (float)g.SCR_HEIGHT, g.projection, g.camera->getViewMatrix());
		vec3 origin = g.rayCasting->getOrigin();
		vec3 direction = g.rayCasting->getDirection();

		//colliding
		bool ret;
		vec3 coords, params, halfParams;
		for (Model m : g.gameMap->getModels()) {
			params = m.getVolume();
			coords = m.getCoords();
			halfParams = params*0.5f;
			coords.x -= halfParams.x;
			coords.z -= halfParams.z;
			ret = g.rayboxCollision->isCollided(coords, coords + params, origin, direction);
			if (ret) {
				cout << "Is collided with " << m.name << endl;
			}
		}

		coords = g.gameMap->getTerrainCoords();
		params = g.gameMap->getTerrainVolume();
		halfParams = params*0.5f;
		coords.x -= halfParams.x;
		coords.z -= halfParams.z;
		cout << coords.x << "," << coords.y << "," << coords.z << ":::" << params.x << "," << params.y << "," << params.z << endl;
		ret = g.rayboxCollision->isCollided(coords, coords + params, origin, direction);
		if (ret) {
			cout << "Is collided with Terrain" << endl;
			vec3 point = g.helper.binSearch(origin, origin + 40.0f*direction, 0, 30, 1.0f);
			cout << "Point: " << point.x << ", " << point.y << ", " << point.z << endl;
			vec2 tile = g.helper.toTileCoords(point, vec3(g.worldOriginX, g.worldOriginY, g.worldOriginZ));
			cout << "Tile: " << tile.x << ", " << tile.y << endl;

			cout << "Cols: " << g.gameMap->getWidth() << ", Rows: " << g.gameMap->getHeight() << endl;

			//path finding
			time_t currTime;
			time(&currTime);
			srand(currTime%UINT_MAX);
			int** matrix;
			int cols = g.gameMap->getHeight(), rows = g.gameMap->getWidth();
			matrix = new int*[rows];
			for (int a = 0; a < rows; ++a) {
				matrix[a] = new int[cols];
				for (int b = 0; b < cols; ++b) {
					matrix[a][b] = rand() % 2;
				}
			}
			vector<Tile> tiles = g.pathfinder->searchPath(matrix,
				rows,
				cols,
				g.player->getTile(),
				Tile(tile));
			if (tiles.size() != 0) {
				vector<unsigned int> indices;
				vector<Vertex> vertices;
				vec3 currPos;
				Vertex v;
				vec3 worldOrigin = vec3(g.getWorldOriginX(), 0.0f, g.getWorldOriginZ());
				for (Tile t : tiles) {
					currPos = g.helper.toWorldCoords(t, vec3(g.worldOriginX, g.worldOriginY, g.worldOriginZ));
					currPos.y = 1.0f;
					v.Position = currPos;
					vertices.push_back(v);
				}

				vector<Vertex> vts;
				for (Vertex v : vertices) {
					v.Position.x -= 1;
					v.Position.z += 1;
					vts.insert(vts.begin(), v);
				}
				g.pathLine->updateVertices(vts);
				g.player->setRoute(tiles, false);
			}
			else
				cout << "No path" << endl;

		}

		//update ray indices and vertices(for line/ray drawing)
		vector<unsigned int> indices;
		vector<Vertex> vertices;
		vec3 currPos = origin;
		Vertex v;
		v.Position = currPos;
		vertices.push_back(v);
		for (int a = 1; a <= 70; ++a) {
			currPos = origin + direction*(float)a;
			v.Position = currPos;
			vertices.push_back(v);
			indices.push_back(a - 1);
			indices.push_back(a);
		}
		g.clickedLine->updateIndices(indices);
		g.clickedLine->updateVertices(vertices);


	}
}
//-------------------

//game lifecycle
//--------------
void GameWorld::start() {
	int ret;
	ret = init();
	if (ret == 1)
		ret = run();
	finish();
}


int GameWorld::init() {
	//* glfw: initialize, configure, create
	// ------------------------------[
	int ret1 = glfwInit();
	if (ret1 != GLFW_TRUE) {
		std::cout << "Failed to init GLFW!"<< std::endl;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}
	glfwMakeContextCurrent(window);
	// --------------------]

	// glew: load all OpenGL function pointers
	// ---------------------------------------[
	GLenum ret;
	if (ret=glewInit())
	{
		std::cout << "Failed to initialize GLEW: " << glewGetErrorString(1) << std::endl;
		system("pause");
		return -1;
	}
	// ---------------------------------------]

	// configure global opengl state
	// -----------------------------[
	glEnable(GL_DEPTH_TEST);
	// -----------------------------]

	// tell GLFW to capture our mouse
	// ------------------------------[
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// ------------------------------]

	//load constants
	// -----------[
	cout << "Settings and Constants: loading..." << endl;
	Constants constants("settings/game.stg", "settings/models.stg");
	if (!constants.isLoaded()) {
		cout << "Settings and Constants: settings can't load." << endl;
		return -1;
	}
	cout << "Settings and Constants: loaded." << endl;
	// -----------]

	// loading models
	//----------------[
	std::cout << "Models: Loading models...\n";
	map<int, pair<string,string>> modelsPaths = constants.getModelsPaths();
	int id;
	string name, path;
	Model terrainModel;
	Model playerModel;
	vector<Model> otherModels;
	for (pair<int,pair<string, string>> pair : modelsPaths) {
		name = pair.second.first;
		path = pair.second.second;
		id = pair.first;
		if (name.compare("Player")==0) {
			playerModel.extractData(path);
			playerModel.name = name;
		}
		else if (name.compare("Terrain") == 0) {
			terrainModel.extractData(path);
			terrainModel.name = name;
		}
		else if (name.compare("Tree") == 0) {
			Model newModel;
			newModel.extractData(path);
			newModel.name = name;
			otherModels.push_back(newModel);
		}
	}
	std::cout << "Models: loaded.\n";
	//----------------]

	

	//set other components
	//---------------------------------------------------------[
	cout << "Other: setting..." << endl;
	vec3 terrainVolume = terrainModel.getVolume();
	vec3 terrainCoords = terrainModel.getCoords();

	rayCasting = new AlgRayCasting();
	rayboxCollision = new AlgRayBoxAABB();
	clickedLine = new SimpleModel(vector<Vertex>(), vector<uint>());
	pathLine = new SimpleModel(vector<Vertex>(), vector<uint>());
	screenBox = new SimpleModel(vector<Vertex>(), vector<uint>());
	pathfinder = new AlgAStarPathfinder();
	int mapWidth = (int)(terrainVolume.z / 2);
	int mapHeight = (int)(terrainVolume.x / 2);
	
	cout << "Other: set." << endl;

	//---------------------------------------------------------]

	//set camera
	//---------------------------------------------------------[
	cout << "Camera: setting...\n";
	worldOriginX = terrainCoords.x - terrainVolume.x*0.5f;
	worldOriginY = 0;
	worldOriginZ = terrainCoords.z - terrainVolume.z*0.5f;
	camera = new Camera(vec3(worldOriginX, 21, worldOriginZ));
	cout << "Camera: set." << endl;
	//---------------------------------------------------------]

	// build and compile shaders
	// -------------------------[
	std::cout << "Shaders: Compiling...\n";
	ourShader = new Shader("vsTextureShader.glsl", "fsTextureShader.glsl", SHADERCOMPLEXITY_FULL);
	gridShader = new Shader("vsGridShader.glsl", "fsGridShader.glsl", SHADERCOMPLEXITY_FULL);
	lineShader = new  Shader("vsLineShader.glsl", "fsLineShader.glsl", SHADERCOMPLEXITY_SIMPLE);
	screenShader = new  Shader("vsScreenShader.glsl", "fsScreenShader.glsl", SHADERCOMPLEXITY_SIMPLE);
	std::cout << "Shaders: compiled.\n";
	// -------------------------]

	// set listeners(callbacks)
	// --------------------[
	cout << "Window callbacks: settings..." << endl;
	glfwSetFramebufferSizeCallback(window, framebuffer_sizeCallback);
	glfwSetCursorPosCallback(window, mouseCursorCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	cout << "Window callbacks: set." << endl;
	// --------------------]

	//last elements(game itself)
	cout << "Game elements: settings..." << endl;
	gameMap = new Map(mapWidth, mapHeight);
	gameMap->addObject(terrainModel);
	player = new Player(playerModel, 0, 0);
	cout << "Game elements: setted." << endl;

	vector<Tile> tiles;
	tiles.push_back(Tile(0, 0));
	tiles.push_back(Tile(1, 0));
	tiles.push_back(Tile(2, 0));
	tiles.push_back(Tile(3, 0));
	tiles.push_back(Tile(4, 0));
	tiles.push_back(Tile(5, 0));
	tiles.push_back(Tile(6, 0));
	tiles.push_back(Tile(7, 0));
	tiles.push_back(Tile(8, 0));
	tiles.push_back(Tile(9, 0));
	player->setRoute(tiles, true);

	return 1;
}

double FRAME_TIME = 1000 / 30;
int GameWorld::run() {
	
	std::cout << "GameLoop started.\n";
	lastFrame = glfwGetTime()*1000;
	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime()*1000;//in miliseconds
		currDeltaTime = currentFrame - lastFrame;
		totalDeltaTime += currDeltaTime;
		lastFrame = currentFrame;
		

		if (totalDeltaTime >= FRAME_TIME) {
			update(totalDeltaTime / FRAME_TIME);
			totalDeltaTime -= FRAME_TIME;
		}

		draw();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		input();
	}
	std::cout << "GameLoop finished.\n";

	return 0;
}
void GameWorld::finish() {
	if(screenShader!=nullptr)
		delete screenShader;
	if (lineShader != nullptr)
		delete lineShader;
	if (gridShader != nullptr)
		delete gridShader;
	if (ourShader != nullptr)
		delete ourShader;
	glDisable(GL_DEPTH_TEST);
	if (screenBox != nullptr)
		delete screenBox;
	if (clickedLine != nullptr)
		delete clickedLine;
	if (rayboxCollision != nullptr)
		delete rayboxCollision;
	if (rayCasting != nullptr)
		delete rayCasting;
	if (camera != nullptr)
		delete camera;
	if (player != nullptr)
		delete player;
	glfwTerminate();
}
//--------------


//per frame work
//--------------------------------
void GameWorld::update(double time) {
	
	player->update(FRAME_TIME);
}
void GameWorld::input() {
	glfwPollEvents();
}
void GameWorld::draw() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gridShader->use();
	//draw terrain
	projection = perspective(radians(camera->getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 140.0f);
	view = camera->getViewMatrix();
	gridShader->setMat4("projection", projection);
	gridShader->setMat4("view", view);
	gridShader->setMat4("model", gameMap->getMatrix("Terrain"));
	gridShader->setFloat("drawGrid", (drawGrid ? 1.0f : 0.0f));
	gameMap->draw(gridShader->get());

	//draw player
	gridShader->setMat4("model", player->getModel().getMatrix());
	player->getModel().draw(gridShader->get());
	//draw trees
	/*for (int a = 0; a < TREE_COUNT; ++a) {
	gridShader->setMat4("model", trees[a].getMatrix());
	trees[a].draw(gridShader->get());
	}*/

	screenShader->use();
	float tmp = sinf(glfwGetTime());
	Vertex vert1, vert2, vert3;
	vert1.Position = vec3(tmp, 0.0f, 0.0f);
	vert2.Position = vec3(0.0f, 0.5f, 0.0f);
	vert3.Position = vec3(0.5f, 0.5f, 0.0f);
	vector<Vertex> vs;
	vs.push_back(vert1);
	vs.push_back(vert2);
	vs.push_back(vert3);
	screenBox->updateVertices(vs);
	//screenBox->draw(screenShader->get(), GL_TRIANGLES);//simple viewport box


													   //draw raycasting vector
	lineShader->use();
	lineShader->setMat4("projection", projection);
	lineShader->setMat4("view", view);
	lineShader->setMat4("model", clickedMatrix);
	clickedLine->draw(lineShader->get(), GL_LINES);

	lineShader->setMat4("projection", projection);
	lineShader->setMat4("view", view);
	lineShader->setMat4("model", mat4(1.0f));
	pathLine->draw(lineShader->get(), GL_LINE_STRIP);

	glfwSwapBuffers(window);
}
//--------------------------------



//TODO delete all code in comment

//vec3 calculateMouseRay(double viewportX, double viewportY) {
//	/*vec4 viewport = vec4(0.0f, 0.0f, SCR_WIDTH, SCR_HEIGHT);
//	vec3 screenPos = vec3(viewportX, SCR_HEIGHT - viewportY - 1, 0.0f);
//
//	vec3 origin = unProject(screenPos, camera.getViewMatrix(), projection, viewport);//near
//	screenPos = vec3(viewportX, SCR_HEIGHT - viewportY - 1, 1.0f);
//	vec3 destiny = unProject(screenPos, camera.getViewMatrix(), projection, viewport);//far
//	vec3 direction = vec3(destiny.x - origin.x, destiny.y - origin.y, destiny.z - origin.z);*/
//	///////////////////////
//
//	//set vertices & indices
//	vector<unsigned int> indices;
//	vector<Vertex> vertices;
//	indices.clear();
//	vertices.clear();
//	vec3 currPos = origin;
//	Vertex v;
//	v.Position = currPos;
//	vertices.push_back(v);
//	for (int a = 1; a <= 200; ++a) {
//		currPos = origin + direction*(float)a;
//		v.Position = currPos;
//		vertices.push_back(v);
//		indices.push_back(a - 1);
//		indices.push_back(a);
//	}
//
//	if (first) {
//		//	setShaderSettings();
//		clickedLine = new SimpleModel(vertices, indices);
//		first = false;
//	}
//	clickedLine->updateIndices(indices);
//	clickedLine->updateVertices(vertices);
//
//	//colliding
//	bool ret = false;
//	vector<Model> collidedObjs;
//	for (Model o : objects) {
//		vec3 oCoords = o.getCoords();//normalize(vec4(player.getCoords(), 1.0f)*player.getMatrix());
//		vec3 oParams = o.getVolume();// normalize(vec4(player.getVolume(), 1.0f));
//									 //ret = collided(oCoords, oCoords + oParams, origin, direction);
//		if (ret) {
//			cout << o.name << " is collided\n";
//			collidedObjs.push_back(o);
//		}
//	}
//
//	if (!collidedObjs.empty()) {
//		float maxy = -1000;
//		Model* selectedObj = nullptr;
//		for (Model m : collidedObjs) {
//			if (maxy < m.getCoords().y)
//			{
//				selectedObj = new Model(m);
//			}
//		}
//
//		if (selectedObj != nullptr) {
//			cout << selectedObj->name << " is selected\n";
//			delete selectedObj;
//		}
//	}
//
//
//	return direction;
//}

//void setShaderSettings() {
//		/*Vertex v1,v2,v3,v4,v5;
//		v1.Position = vec3(0.13f, 1.0f, 7.0f);
//		v2.Position = vec3(0.5f, 0.3f, 3.0f);
//		v3.Position = vec3(-0.5f, 0.2f, 4.0f);
//		v4.Position = vec3(0.8f, 0.7f, 0.1f);
//		v5.Position = vec3(0.0f, 0.0f, 0.0f);
//		vertices.push_back(v1);
//		vertices.push_back(v2);
//		vertices.push_back(v3);
//		vertices.push_back(v4);
//		vertices.push_back(v5);
//		uint i1, i2, i3,i4;
//		i1 = 0;
//		i2 = 1;
//		i3 = 2;
//		i4 = 3;
//		indices.push_back(i1);
//		indices.push_back(i2);
//		indices.push_back(i3);
//		indices.push_back(i4);*/
//		//for (unsigned int a : player.getIndices()) {
//		//	//indices.push_back(a);
//		//	indices.insert(0, a);
//		//}
//		//for (Vertex v : player.getVertices()) {
//		//	//vertices.push_back(v);
//		//	vertices._Insert(0, v);
//		//}
//		/*Vertex originPoint;
//		originPoint.Position = camera.position;
//		vertices.push_back(originPoint);
//		vertices.push_back(originPoint);
//		indices.push_back(0);
//		indices.push_back(1);*/
//
//		// create buffers/arrays
//glGenVertexArrays(1, &vao);
//glGenBuffers(1, &vbo);
//glGenBuffers(1, &ibo);
//
//glBindVertexArray(vao);
//// load data into vertex buffers
//glBindBuffer(GL_ARRAY_BUFFER, vbo);
//// A great thing about structs is that their memory layout is sequential for all its items.
//// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a vec3/2 array which
//// again translates to 3/2 floats which translates to a byte array.
//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//
//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
//
//// set the vertex attribute pointers
//// vertex Positions
//glEnableVertexAttribArray(0);
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//
//glBindVertexArray(0);
//	}
//	void drawShaderSettings() {
//
//		// draw mesh
//		if (vertices.size() == 0) return;
//		glBindVertexArray(vao);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
//		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//
//	}
//	void setScreenShaderSettings() {
//		std::cout << "Size before:" << screenIndices.size() << std::endl;
//
//		// create buffers/arrays
//		glGenVertexArrays(1, &vao2);
//		glGenBuffers(1, &vbo2);
//		glGenBuffers(1, &ibo2);
//
//		glBindVertexArray(vao2);
//		// load data into vertex buffers
//		glBindBuffer(GL_ARRAY_BUFFER, vbo2);
//		// A great thing about structs is that their memory layout is sequential for all its items.
//		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a vec3/2 array which
//		// again translates to 3/2 floats which translates to a byte array.
//		glBufferData(GL_ARRAY_BUFFER, screenVertices.size() * sizeof(Vertex), &screenVertices[0], GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, screenIndices.size() * sizeof(unsigned int), &screenIndices[0], GL_STATIC_DRAW);
//
//		// set the vertex attribute pointers
//		// vertex Positions
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//
//		glBindVertexArray(0);
//
//	}
//	void drawScreenShaderSettings() {
//
//		// draw mesh
//		glBindVertexArray(vao2);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*screenVertices.size(), &screenVertices[0], GL_DYNAMIC_DRAW);
//		glDrawElements(GL_TRIANGLES, screenIndices.size(), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//
//	}