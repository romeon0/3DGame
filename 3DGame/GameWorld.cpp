
#define GL_GLEXT_PROTOTYPES
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <iostream>
#include <thread>
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
#include "AlgBoxBoxAABB.h"
#include "GameWorld.h"
#include "Constants.h"
#include "AnimatedModel.h"
#include "Ammo.h"
#include "Enemy.h"
#include <Cairo\cairo.h>
#include "CollideTesting.h"
#include "ProgressBar.h"

using glm::vec3;
using glm::mat4;
using std::stringstream;
using namespace std::this_thread;

vector<Vertex> vts;
vector<unsigned int> ids;
int** matrix;
Vertex v1, v2, v3, v4;
Ammo* ammo=NULL;
float rotRadian = 0.0f;

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
	float deltaTime = g.currentDeltaTime;
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
}
//-process keyboard


void GameWorld::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GameWorld& g = GameWorld::getInstance();
	float deltaTime = g.currentDeltaTime;
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
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		g.drawGrid = !g.drawGrid;
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
	//	mat4 matrix = g.model->getRotationMatrix();
	//	matrix = rotate(matrix, radians(45.0f), vec3(1.0f, 0.0f, 0.0f));
		g.player->getModel().rotateDegree(45.0f,1);
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		mat4 matrix = mat4(1.0f);
		matrix = rotate(matrix, radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
	//	g.model->setRotationMatrix(matrix);
		g.player->getModel().rotateDegree(45.0f, 2);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		mat4 matrix = mat4(1.0f);
		matrix = rotate(matrix, radians(45.0f), vec3(0.0f, 0.0f, 1.0f));
		//g.model->setRotationMatrix(matrix);
		g.player->getModel().rotateDegree(45.0f, 3);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		//g.model->setSelected(!g.model->isSelected());
		ammo->getModel().rotateDegree(30, 1);
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		//g.model->setSelected(!g.model->isSelected());
		ammo->getModel().rotateDegree(60, 2);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		g.model->setSelected(!g.model->isSelected());
		ammo->getModel().rotateRad(rotRadian, 2);
	}
}
void GameWorld::framebuffer_sizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	GameWorld& g = GameWorld::getInstance();
	g.SCR_HEIGHT = height;
	g.SCR_WIDTH = width;
}
void  GameWorld::mouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (window == NULL) {
		cout << "ERROR::mouseCursorCallback: Null window!\n";
		return;
	}

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
		Helper h;
		glfwGetCursorPos(window, &viewportX, &viewportY);

		g.selectedEnemy = NULL;
		g.textManager->blockDraw("EnemyInfo");

		//calculate vector origin & direction
		g.rayCasting->calculate(viewportX, viewportY, g.SCR_WIDTH, g.SCR_HEIGHT, g.projection, g.camera->getViewMatrix());
		vec3 origin = g.rayCasting->getOrigin();
		vec3 direction = g.rayCasting->getDirection();

		g.terrainAttacked(origin, direction);

		//-------- detecting colliding
		//detect map object colliding
		map<double, Model*> collidedMapObjects = g.checkRayWithMapObjs(origin, direction);
		//detect colliding with enemies
		map<double, Enemy*> collidedEnemies = g.checkRayWithEnemies(origin, direction);
		//detect Player colliding
		pair<bool, double> collidedWithPlayer = g.checkRayWithPlayer(origin, direction);
		//detect Terrain colliding
		bool collidedWithMap = g.checkRayWithTerrain(origin, direction);
		//-----------End colliding detection

		//--------SELECTING
		//select selected object\map\player\enemy
		double minTNear = 1000.0f;
		int type = -1;
		for (pair<double, Enemy*> pair : collidedEnemies) {//check colliding with enemies
			double tNear = pair.first;
			if (tNear < minTNear) {
				minTNear = tNear;
				type = 0;
			}
		}
		for (pair<double, Model*> pair : collidedMapObjects) {//check colliding with mapObjects
			double tNear = pair.first;
			if (tNear < minTNear) {
				minTNear = tNear;
				type = 1;
			}
		}
		if (collidedWithPlayer.second < minTNear) {//collided with player
			type = 2;
		}
		if (type == -1 && collidedWithMap)//collided with terrain
			type = 3;

		cout << "Type: " << type << endl;
		switch (type) {
		case 0://collided with enemy
			//collidedEnemies.at(minTNear)->getModel().setSelected(true);
			g.enemyAttacked(collidedEnemies.at(minTNear));
			break;
		case 1://collided with map object
			collidedMapObjects.at(minTNear)->setSelected(true);
			break;
		case 2://collided with player
			g.player->getModel().setSelected(true);
			break;
		case 3://collided with map
			break;
		case -1://no collide
			break;
		}


		/*
		if (type == 3) {//collided with MAP. Search path for player
			cout << "Is collided with Terrain" << endl;
			vec3 point = g.helper.binSearch(origin, origin + 40.0f*direction, 0, 30, 1.0f);
			vec2 tile = g.helper.toTileCoords(point, g.getWorldOrigin());
			cout << "Clicked on Coord " << point.x << ", " << point.z << endl;
			cout << "Clicked on tile " << tile.x << ", " << tile.y << endl;

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
					matrix[a][b] = 0;// rand() % 2;
				}
			}
			vector<Tile> tiles = g.pathfinder->searchPath(g.gameMap->getGridPatency(),
				rows,
				cols,
				g.player->getTile(),
				Tile(tile));
			for (Tile t : tiles) {
				cout << "Tile: " << t.getX() << ", " << t.getY() << endl;
			}
			if (tiles.size() != 0) {//if path found
				vector<unsigned int> indices;
				vector<Vertex> vertices;
				vec3 currPos;
				Vertex v;
				vec3 worldOrigin = g.getWorldOrigin();
				cout << "WorldORIGIN: ";
				h.showVector(worldOrigin);
				cout << "TerrainVolume: ";
				h.showVector(g.gameMap->getTerrainVolume());

				for (Tile t : tiles) { 
					currPos = g.helper.toWorldCoords(t, worldOrigin,g.gameMap->getTerrainVolume());
					cout << "CurrPosition: " << currPos.x << ", " << currPos.z << endl;
					v.Position = currPos;
				//	v.Position.x -= 1;
					v.Position.y = 3;// += 10;
			//		v.Position.z += 1;
					vertices.insert(vertices.begin(), v);
				}

				g.pathLine->updateVertices(vertices);
				g.player->setRoute(tiles, false);
			}
			else
				cout << "No path" << endl;//not path between these tiles

		}
		*/

		//collided with MAP. Search path for player
		if (type == 3) {
			cout << "Is collided with Terrain" << endl;
			vec3 point = g.helper.binSearch(origin, origin + 40.0f*direction, 0, 30, 1.0f);
			vec2 tile = g.helper.toTileCoords(point, g.getWorldOrigin()- g.gameMap->getTerrainVolume());
			cout << "Clicked on Coord " << point.x << ", " << point.z << endl;
			cout << "Clicked on tile " << tile.x << ", " << tile.y << endl;

			//path finding
			
			vector<Tile> tiles = g.pathfinder->searchPath(g.gameMap->getGridPatency(),
				g.gameMap->getHeight(),
				g.gameMap->getWidth(),
				g.player->getTile(),
				Tile(tile));
			for (Tile t : tiles) {
				cout << "Tile: " << t.getX() << ", " << t.getY() << endl;
			}

			if (tiles.size() != 0) {//if path found
				vector<unsigned int> indices;
				vector<Vertex> vertices;
				vec3 currPos;
				Vertex v;
				vec3 worldOrigin = g.getWorldOrigin();
				cout << "WorldORIGIN: ";
				h.showVector(worldOrigin);
				cout << "TerrainVolume: ";
				h.showVector(g.gameMap->getTerrainVolume());

				for (Tile t : tiles) {
					currPos = g.helper.toWorldCoords(t, worldOrigin, g.gameMap->getTerrainVolume());
					cout << "CurrPosition: " << currPos.x << ", " << currPos.z << endl;
					v.Position = currPos;
					v.Position.x -= 1;
					v.Position.y = 10;// += 10;
					v.Position.z -= 1;
					vertices.insert(vertices.begin(), v);
				}

				g.pathLine->updateVertices(vertices);
				g.player->setRoute(tiles, true);
			}
			else
				cout << "No path" << endl;//not path between these tiles

		}

		//update ray indices and vertices(for line/ray drawing)
		vector<unsigned int> indices;
		vector<Vertex> vertices;
		Vertex v;
		v.Position = origin;
		vertices.push_back(v);
		v.Position = origin + direction*80.0f;
		vertices.push_back(v);
		g.clickedLine->updateVertices(vertices);
	}
}

map<double, Enemy*> GameWorld::checkRayWithEnemies(vec3 origin, vec3 direction) {
	map<double, Enemy*> collidedEnemies;
	for (int nrEnemy = 0; nrEnemy < enemies.size(); ++nrEnemy) {
		Enemy* e = enemies.at(nrEnemy);
		Model& m = e->getModel();
		vec3 volume = m.getVolume();
		vec3 coords = m.getCoords();
		coords -= volume / 2.0f;
		m.setSelected(false);
		bool collidedWithEnemy = rayboxCollision->isCollided(coords, volume, origin, direction);
		if (collidedWithEnemy) {
			cout << "Is collided with " << "enemy" << endl;
			double tnear = rayboxCollision->getTNear();
			collidedEnemies.insert(make_pair(tnear, e));
		}
	}
	return collidedEnemies;
}
map<double, Model*> GameWorld::checkRayWithMapObjs(vec3 origin, vec3 direction) {
	map<double, Model*> collidedMapObjects;
	for (Model& m : gameMap->getModels()) {
		vec3 volume = m.getVolume();
		vec3 coords = m.getCoords();
		coords -= volume / 2.0f;

		m.setSelected(false);
		bool collidedWithMapObject = rayboxCollision->isCollided(coords, volume, origin, direction);
		if (collidedWithMapObject) {
			cout << "Is collided with obj " << m.getName() << endl;
			double tnear = rayboxCollision->getTNear();
			collidedMapObjects.insert(make_pair(tnear, &m));
		}
	}
	return collidedMapObjects;
}
pair<bool, double> GameWorld::checkRayWithPlayer(vec3 origin, vec3 direction) {
	pair<bool, double> result;
	Model& m = player->getModel();
	vec3 volume = m.getVolume();
	vec3 coords = m.getCoords();
	coords -= volume / 2.0f;
	m.setSelected(false);
	bool collidedWithPlayer = rayboxCollision->isCollided(coords, volume, origin, direction);
	double playerTNear = 1000.0;
	if (collidedWithPlayer) {
		playerTNear = rayboxCollision->getTNear();
		cout << "Is collided with " << "player" << endl;
	}
	result.first = collidedWithPlayer;
	result.second = playerTNear;
	return result;
}
bool GameWorld::checkRayWithTerrain(vec3 origin, vec3 direction) {
	//detect terrain coliding
	vec3 coords = gameMap->getTerrainCoords();
	vec3 volume = gameMap->getTerrainVolume();
	coords -= volume / 2.0f;
	bool collidedWithMap = rayboxCollision->isCollided(coords, volume, origin, direction);
	if (collidedWithMap) {
		cout << "Is collided with " << "map" << endl;
	}

	return false;// collidedWithMap;
}

//-------------------

void GameWorld::enemyAttacked(Enemy* enemy) {
	enemy->getModel().setSelected(true);
	textManager->allowDraw("EnemyInfo");
	Model model("models/ammo.dae");
	AmmoProperty* ammoProperty = new AmmoProperty(1, ".43 Ammo", "This is ammo .44", 30.0, 0.2, 40);
	Ammo* ammo = new Ammo(model, ammoProperty,player->getModel().getCoords(),enemy->getModel().getCoords());
	ammoManager->addAmmo(ammo);

	selectedEnemy = enemy;	
	cout << "Selected enemy: " << selectedEnemy->getName() << endl;
}
void GameWorld::terrainAttacked(vec3 origin, vec3 direction) {
	Model model("models/ammo.dae");
	AmmoProperty* ammoProperty = new AmmoProperty(1,".43 Ammo","This is ammo .44",30.0,0.2,40);
	Helper h;

	vec3 vect1 = direction - origin;
	vec3 vect2 = vec3(0,0,100.0f);
	float degree = h.calcDegreeBetween(vect1, vect2);
	cout << "Degree: " << degree << endl;

	model.rotateRad(degree,2);
	//new Ammo(model, ammoProperty, vec3(-50, 5.0f, -50), vec3(0, 5.0f, 0));//
	Ammo* ammo = new Ammo(model, ammoProperty, origin, direction);
	ammoManager->addAmmo(ammo);
}

//game lifecycle
//--------------

void GameWorld::start() { 
	int ret;
	ret = init();
	if (ret == 1)
	ret = run();
	finish();

	/*init();
	AnimatedModel* model = new AnimatedModel();
	model->initPose("models/anim_man2.dae");
	finish();
	system("pause");*/

	////////////////////////////////////////
	/*cairo_surface_t *surface;
	cairo_t *cr;

	surface = cairo_image_surface_create_from_png("newImg.png");
	//surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,100, 20);
	cr = cairo_create(surface);
	
	//fill background
	cairo_rectangle(cr, 0, 0, 0.5, 0.5);
	cairo_set_source_rgb(cr, 0.2, 0.9, 0.2);
	cairo_fill(cr);

	//draw text
	cairo_set_source_rgb(cr, 0.2, 0.8, 0.3);
	cairo_select_font_face(cr, "Verdana",
		CAIRO_FONT_SLANT_NORMAL,
		CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 13);
	cairo_move_to(cr, 0, 10);
	cairo_show_text(cr, "2000/3670");

	cairo_surface_write_to_png(surface, "newImg.png");

	cairo_surface_destroy(surface);
	cairo_destroy(cr);*/

	//init
/*	Model model("models/ammo.dae");
	AmmoProperty property;
	map<int, Model> models;
	map<int, AmmoProperty> properties;
	ammoManager = new AmmoManager(models,properties);
	//create
	Ammo* ammo1 = new Ammo(model, property, vec3(0,0,0.0f), vec3(1000.0, 0, 0.0f));
	Ammo* ammo2 = new Ammo(model, property, vec3(0, 0, 0.0f), vec3(100.0, 0, 0.0f));
	Ammo* ammo3 = new Ammo(model, property, vec3(0, 0, 0.0f), vec3(0, 0, 0.0f));
	Ammo* ammo4 = new Ammo(model, property, vec3(0, 0, 0.0f), vec3(0, 0, 0.0f));
	Enemy* enemy1 = new Enemy(model, 0, 0);
	Enemy* enemy1 = new Enemy(model, 0, 0);
	Enemy* enemy1 = new Enemy(model, 0, 0);
	Enemy* enemy1 = new Enemy(model, 0, 0);
	ammoManager->addAmmo(ammo1);
	ammoManager->addAmmo(ammo2);
	ammoManager->addAmmo(ammo3);
	ammoManager->addAmmo(ammo4);
	//test
	CollideTesting* testing = new CollideTesting();
	
	testing->test(ammoManager, enemies);

	system("pause");
	*/
}


int GameWorld::init() {

	//* glfw: initialize, configure, create
	// ------------------------------[
	int ret = glfwInit();
	if (ret != GLFW_TRUE) {
		std::cout << "Failed to init GLFW!" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Application", NULL, NULL);
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
	if (ret = glewInit())
	{
		std::cout << "Failed to initialize GLEW: " << glewGetErrorString(1) << std::endl;
		system("pause");
		return -1;
	}
	// ---------------------------------------]

	glViewport(0, 0, 800, 600);
	glLineWidth(3);
	glColor3f(0.3, 0.4, 0.9);
	// configure global opengl state
	// -----------------------------[
	glEnable(GL_DEPTH_TEST);
	glColor3b(50, 50, 200);
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

	//Loading models
	//----------------[
	//models from file
	std::cout << "Models: Loading models...\n";
	map<int, pair<string, string>> modelsPaths = constants.getModelsPaths();
	int id;
	string name, path;
	Model terrainModel;
	Model playerModel;
	vector<Model> otherModels;
	for (pair<int, pair<string, string>> pair : modelsPaths) {
		name = pair.second.first;
		path = pair.second.second;
		id = pair.first;
		if (name.compare("Player") == 0) {
			playerModel.extractData(path);
			playerModel.setName(name);
		}
		else if (name.compare("Terrain") == 0) {
			terrainModel.extractData(path);
			terrainModel.setName(name);
		}
		else if (name.compare("Tree") == 0) {
			Model newModel;
			newModel.extractData(path);
			newModel.setName(name);
			otherModels.push_back(newModel);
		}
	}
	//simple models(no texture, just points)
	vector<unsigned int> indices;
	vector<Vertex> vertices;
	Vertex v;
	indices.push_back(0);
	indices.push_back(1);
	clickedLine = new SimpleModel(vector<Vertex>(), indices);
	pathLine = new SimpleModel(vector<Vertex>(), vector<unsigned int>());
	screenBox = new SimpleModel(vector<Vertex>(), vector<unsigned int>());
	mouseModel = new SimpleModel(vector<Vertex>(), vector<unsigned int>(), "models/cursor_target.png");
	simpleCube = new SimpleModel(vector<Vertex>(), vector<unsigned int>());
	vertices.clear();
	indices.clear();
	v.Position = vec3(0.0, -100.0, 0.0f);
	vertices.push_back(v);
	v.Position = vec3(0.0, 100.0, 0.0f);
	vertices.push_back(v);
	indices.push_back(0);
	indices.push_back(1);
	centerLine = new SimpleModel(vertices, indices);
	//trees
  /*float worldX = worldOriginX + 1;
	float worldZ = worldOriginZ + 1;
	for (int a = 2; a <= 2; ++a) {
		Model tree = otherModels.at(0);
		tree.goTo(0.0f, 50.0f + a * 4, 0.0f);
		string name = "Tree";
		name.insert(name.end(), a + '0');
		tree.setName(name);
		gameMap->addObject(tree);
	}*/
	std::cout << "Models: loaded.\n";
	//----------------]

	//set world origins
	//------------------------[
	vec3 terrainVolume = terrainModel.getVolume();
	vec3 terrainCoords = terrainModel.getCoords();
	int mapWidth = terrainVolume.z;//(int)(terrainVolume.z / 2);
	int mapHeight = terrainVolume.x;// (int)(terrainVolume.x / 2);
	worldOriginX = terrainCoords.x - terrainVolume.x*0.5f;
	worldOriginY = terrainCoords.y + terrainVolume.y;
	worldOriginZ = terrainCoords.z - terrainVolume.z*0.5f;
	cout << "Terrain volume: " << terrainVolume.x 
		<< ", " << terrainVolume.y  
		<< ", "<< terrainVolume.z << endl;
	cout << "MapDimensions: " << mapWidth << ", " << mapHeight << endl;
	cout << "WorldOrigin: " << worldOriginX << ", " << worldOriginY << ", " << worldOriginZ << endl;
	//------------------------]

	//Init game elements
	//----------------------[
	//Init map
	cout << "Game elements: settings..." << endl;
	cout << "Map initializing" << endl;
	gameMap = new Map(mapWidth, mapHeight);
	gameMap->addObject(terrainModel);
	cout << "Map initialized." << endl;
	//Init player
	cout << "Player initializing" << endl;
	player = new Player(playerModel, 1, 1);
	cout << "Player initialized." << endl;
	//Init algorithms
	cout << "Algorithms initializing" << endl;
	rayCasting = new AlgRayCasting();
	rayboxCollision = new AlgRayBoxAABB();
	pathfinder = new AlgAStarPathfinder();
	cout << "Algorithms initialized." << endl;
	//Init camera
	cout << "Camera initializing" << endl;
	camera = new Camera(vec3(worldOriginX, 0, worldOriginZ + 50));//worldOriginX, 21, worldOriginZ
	cout << "Camera initialized." << endl;
	cout << "Game elements: setted." << endl;
	//Init mouse
	indices.clear();
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(2); indices.push_back(3); indices.push_back(0);
	mouseModel->updateIndices(indices);
	//Init Ammo Manager
	ammoManager = new AmmoManager(constants.getAmmosModels(), constants.getAmmosProperties());
	//----------------------]

	// build and compile shaders
	// -------------------------[
	std::cout << "Shaders: Compiling...\n";
	ourShader = new Shader("vsTextureShader.glsl", "fsTextureShader.glsl", SHADERCOMPLEXITY_FULL);
	gridShader = new Shader("vsGridShader.glsl", "fsGridShader.glsl", SHADERCOMPLEXITY_FULL);
	lineShader = new  Shader("vsLineShader.glsl", "fsLineShader.glsl", SHADERCOMPLEXITY_SIMPLE);
	primitiveScreenShader = new  Shader("vsPrimitiveScreenShader.glsl", "fsPrimitiveScreenShader.glsl", SHADERCOMPLEXITY_SIMPLE);
	animShader = new  Shader("vsAnimShader.glsl", "fsAnimShader.glsl", SHADERCOMPLEXITY_SIMPLE);
	outlineShader = new  Shader("vsOutlineShader.glsl", "fsOutlineShader.glsl", SHADERCOMPLEXITY_SIMPLE);
	colorShader = new  Shader("vsColorShader.glsl", "fsColorShader.glsl", SHADERCOMPLEXITY_SIMPLE);
	mouseShader = new  Shader("vsMouseShader.glsl", "fsMouseShader.glsl", SHADERCOMPLEXITY_SIMPLE);
	std::cout << "Shaders: compiled.\n";
	// -------------------------]

	// set listeners(callbacks)
	// --------------------[
	cout << "Window callbacks: setting..." << endl;
	glfwSetFramebufferSizeCallback(window, framebuffer_sizeCallback);
	glfwSetCursorPosCallback(window, mouseCursorCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	cout << "Window callbacks: set." << endl;
	// --------------------]


	Model enemyModel("models/anim_cube.dae");
	Enemy* enemy1 = new Enemy(enemyModel, 13, 3);
	Enemy* enemy2 = new Enemy(enemyModel, 20, 20);
	Enemy* enemy3 = new Enemy(enemyModel, 20, 12);
	enemy1->setName("Enemy1");
	enemy2->setName("Enemy2");
	enemy3->setName("Enemy3");
	enemies.push_back(enemy1);
	enemies.push_back(enemy2);
	enemies.push_back(enemy3);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);
	enemyInfo = new SimpleModel(vector<Vertex>(), indices, "enemy_info.png");
	initGUIImage(*enemyInfo, SCR_WIDTH - 200, SCR_HEIGHT - 100, 200, 100);
	mainPanel = new SimpleModel(vector<Vertex>(), indices, "main_panel.png");
	initGUIImage(*mainPanel, 50, 50, 230, 62);
	playerInfo = new SimpleModel(vector<Vertex>(), indices, "player_Info.png");
	initGUIImage(*playerInfo, SCR_WIDTH - 255, 5, 250, 100);

	textManager = new TextManager();
	SimpleModel* verdanaFont = new SimpleModel(vertices, indices, "fonts/VerdanaFont.bmp");
	SimpleModel* verdanaFont2 = new SimpleModel(vertices, indices, "fonts/VerdanaFont.bmp");
	textManager->addFont("Verdana", verdanaFont);
	double enemyInfoTextPosX = (SCR_WIDTH - 185);
	double enemyInfoTextPosY = (SCR_HEIGHT - 26);
	textManager->addText("EnemyInfo", "Hello there qwerty!", "Verdana", enemyInfoTextPosX, enemyInfoTextPosY, 20, 20, SCR_WIDTH, SCR_HEIGHT);
	/*textManager->addText("EnemyInfo2", "Hello there1!", "Verdana", 0, 0, 20, 30, SCR_WIDTH, SCR_HEIGHT);
	textManager->addText("EnemyInfo3", "Hello there3!", "Verdana", 300, 300, 20, 20, SCR_WIDTH, SCR_HEIGHT);
	textManager->addText("EnemyInfo4", "Hello there4!", "Verdana", 200, 100, 30,20, SCR_WIDTH, SCR_HEIGHT);
	textManager->addText("EnemyInfo5", "Hello there5!", "Verdana", 100, 300, 10,10, SCR_WIDTH, SCR_HEIGHT);*/

	bar = new ProgressBar(SCR_WIDTH-247, 62, 232, 28, SCR_WIDTH, SCR_HEIGHT, 300);
	animModel = new AnimatedModel("models/anim_man2.dae");
	return 1;
}


long long nextTime;
int GameWorld::run() {
	Helper h;
	std::cout << "GameLoop started.\n";
	//lastFrameTime = getCurrentTime();
	//totalDeltaTime = 0;
	/*timePoint start = getCurrentTime();
	for (long long a = 0; a < 6059334; ++a) {
		if (a % 100 == 0) totalDeltaTime += 1;
	}
	timePoint end = getCurrentTime();
	long long nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	long long miliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	int seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	int minutes = std::chrono::duration_cast<std::chrono::minutes>(end - start).count();
	int hours = std::chrono::duration_cast<std::chrono::hours>(end - start).count();

	cout << "nanoseconds: " << nanoseconds << endl;
	cout << "Micro: " << microseconds << endl;
	cout << "miliseconds: " << miliseconds << endl;
	cout << "seconds: " << seconds << endl;
	cout << "minutes: " << minutes << endl;
	cout << "hours: " << hours << endl;
	system("pause");*/

	//lastFrameTime = getCurrentTime();
nextTime = h.getCurrentTime();
lastTime = h.getCurrentTime();
currentTime = h.getCurrentTime();



while (!glfwWindowShouldClose(window))
{
	/*float currentFrameTime = (float)glfwGetTime() * 1000.0f;//miliseconds
	currDeltaTime = currentFrameTime - lastFrameTime;
	totalDeltaTime += currDeltaTime;
	lastFrameTime = currentFrameTime;*/
	//timePoint currentFrameTime = getCurrentTime();//abstract time
	//float currDeltaTime = getDeltaTime(lastFrameTime, currentFrameTime);//miliseconds
	//totalDeltaTime += currDeltaTime;//miliseconds


	//if (totalDeltaTime >= FRAME_TIME) {
	//	cout << "currDeltaTime: " << currDeltaTime << endl;
	//	cout << "FRAME_TIME: " << FRAME_TIME << endl;
	//	cout << "totalDeltaTime1: " << totalDeltaTime << endl;
	//	totalDeltaTime = 0;
	//	cout << "totalDeltaTime2: " << totalDeltaTime << endl;
	//}

	lastTime = currentTime;
	currentTime = h.getCurrentTime();

	currentDeltaTime = currentTime - lastTime;
	update(currentDeltaTime);
	collide();

	input();
	draw();
}
std::cout << "GameLoop finished.\n";

return 0;
}
void GameWorld::finish() {
	if (primitiveScreenShader != nullptr)
		delete primitiveScreenShader;
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
//@param time - current time in miliseconds
double cFrameTime = 0.0f;
double frameTime = 500;//40.0f;//25 frames per second(1000/25)
void GameWorld::update(double timeElapsed) {
	//	cout << "Time elapsed: " << timeElapsed << endl;
	//	cout << "currTime: "<<currTime<<"\n";
	player->update(timeElapsed);
	//animator->update(currTime);

	//update animations
//	model->update(timeElapsed);

	animModel->update(timeElapsed);

	if (ammo != NULL)
		ammo->update(timeElapsed);

	ammoManager->update(timeElapsed);
}
void GameWorld::input() {
	glfwPollEvents();
}

void GameWorld::collide() {
	AlgBoxBoxAABB checker;
	vector<Ammo*> toDelete;
	bool collided = false;

	list<Ammo*> ammos = ammoManager->getAmmos();
	for (auto it = ammos.begin(); it!=ammos.end(); ++it) {
		Ammo* ammo = (Ammo*)*it;
		vec3 coordsBody1 = ammo->getModel().getCoords();
		vec3 volumeBody1 = ammo->getModel().getVolume();
		coordsBody1 -= volumeBody1 / 2.0f;

		//is collided with enemy?
		for (Enemy* enemy : enemies) {
			vec3 coordsBody2 = enemy->getModel().getCoords();
			vec3 volumeBody2 = enemy->getModel().getVolume();
			coordsBody2 -= volumeBody2 / 2.0f;
			bool collidedWithEnemy = checker.checkCollide(coordsBody1, coordsBody1 + volumeBody1,
				coordsBody2, coordsBody2 + volumeBody2);
			if (collidedWithEnemy) {
				cout << "Collided with enemy" << endl;
				toDelete.push_back(ammo);
				collided = true;

				enemy->damage(ammo->getProperty().getDamage());
			}
		}

		//is collided with player?
		vec3 coordsBody2 = player->getModel().getCoords();
		vec3 volumeBody2 = player->getModel().getVolume();
		coordsBody2 -= volumeBody2 / 2.0f;
		bool collidedWithPlayer = checker.checkCollide(coordsBody1, coordsBody1 + volumeBody1,
			coordsBody2, coordsBody2 + volumeBody2);
		if (collidedWithPlayer) {
			cout << "Collided with player" << endl;
			toDelete.push_back(ammo);
			collided = true;
			player->damage(40.0);
			bar->add(-40);
		}
	}

	if (collided) {
		for (Ammo* ammo : toDelete) {
			ammoManager->removeAmmo(ammo);
		}
	}

	
	//else
	//	cout << "Not collided\n";
}

void GameWorld::drawWithOutline(AnimatedModel* model) {

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//drawing to stencil
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	glStencilMask(0xFF); // Write to stencil buffer
	gridShader->use();
	gridShader->setMat4("projection", projection);
	gridShader->setMat4("view", view);
	gridShader->setMat4("model", model->getMatrix());
	model->draw(gridShader->get());

	//drawing border
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Pass test if stencil value is 1
										 //glStencilMask(0x00); // Don't write anything to stencil buffer
	glDisable(GL_DEPTH_TEST);
	mat4 m = glm::scale(model->getMatrix(), glm::vec3(1.08f, 1.0001f, 1.08f));
	outlineShader->use();
	outlineShader->setMat4("projection", projection);
	outlineShader->setMat4("view", view);
	outlineShader->setMat4("model", m);
	model->draw(outlineShader->get());

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
}

void GameWorld::drawWithOutline(Model* model, vec3 color) {
	glEnable(GL_STENCIL_TEST);//enable stencil test

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//if depth&stencip passed -> replace value
	//push to stencil
	glStencilFunc(GL_ALWAYS, 1, 0xFF);//always write 1 
	glStencilMask(0xFF);//allow write to stencil
	gridShader->use();
	gridShader->setMat4("projection", projection);
	gridShader->setMat4("view", view);
	gridShader->setMat4("model", model->getMatrix());
	model->draw(gridShader->get());

	//drawing border
	glStencilMask(0xFF);//here we need to put 0x00 mask to restrict writing in stencil, but it working only with 0xFF...
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);//draw only diference between these models(scalling difference)
	glDisable(GL_DEPTH_TEST);
	//glStencilMask(0);
	mat4 m = glm::scale(model->getMatrix(), glm::vec3(1.08f, 1.001f, 1.08f)); 
	outlineShader->use();
	outlineShader->setMat4("projection", projection);
	outlineShader->setMat4("view", view);
	outlineShader->setMat4("model", m);
	outlineShader->setVec3("color", color);
	model->draw(outlineShader->get());

	//clearing
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);//set default operation(in any case keep current values)
	glStencilFunc(GL_ALWAYS, 0, 0x0);//aw
	glClear(GL_STENCIL_BUFFER_BIT);
	glClearStencil(0);//clearing stencil, writing with 0 values
	glDisable(GL_STENCIL_TEST);//disable stencil buffer
	glStencilMask(0x00);//restrict writing in stencil buffer
	glEnable(GL_DEPTH_TEST);//enabline depth testing
	glDepthMask(0xFF);
	//glDepthMask(0xFF);
}

void GameWorld::drawSimpleCube(vec3 min, vec3 volume) {
	/*
	V4 <-----V3
	|     |
	V1->-----^V2
	*/
	float width = volume.x ;
	float height = volume.y;
	float depth = volume.z;
	Vertex vert1, vert2, vert3, vert4;
	Vertex vert5, vert6, vert7, vert8;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	
	vert1.Position = vec3(min.x, min.y, min.z);
	vert2.Position = vec3(min.x + width, min.y, min.z);
	vert3.Position = vec3(min.x + width, min.y+height, min.z);
	vert4.Position = vec3(min.x, min.y+height, min.z);
	vert5.Position = vec3(min.x, min.y, min.z + depth);
	vert6.Position = vec3(min.x + width, min.y, min.z + depth);
	vert7.Position = vec3(min.x + width, min.y + height, min.z + depth);
	vert8.Position = vec3(min.x, min.y + height, min.z+depth);

	vertices.push_back(vert1);
	vertices.push_back(vert2);
	vertices.push_back(vert3);
	vertices.push_back(vert4);
	vertices.push_back(vert5);
	vertices.push_back(vert6);
	vertices.push_back(vert7);
	vertices.push_back(vert8);

	/*//front
	vertices.push_back(vert1);
	vertices.push_back(vert2);
	vertices.push_back(vert3);
	vertices.push_back(vert3);
	vertices.push_back(vert4);
	vertices.push_back(vert1);
	//back
	vertices.push_back(vert5);
	vertices.push_back(vert6);
	vertices.push_back(vert7);
	vertices.push_back(vert7);
	vertices.push_back(vert8);
	vertices.push_back(vert5);
	//left
	vertices.push_back(vert1);
	vertices.push_back(vert5);
	vertices.push_back(vert8);
	vertices.push_back(vert8);
	vertices.push_back(vert4);
	vertices.push_back(vert1);
	//right
	vertices.push_back(vert2);
	vertices.push_back(vert6);
	vertices.push_back(vert7);
	vertices.push_back(vert7);
	vertices.push_back(vert3);
	vertices.push_back(vert2);
	//top
	vertices.push_back(vert4);
	vertices.push_back(vert8);
	vertices.push_back(vert7);
	vertices.push_back(vert7);
	vertices.push_back(vert3);
	vertices.push_back(vert4);
	//bottom
	vertices.push_back(vert1);
	vertices.push_back(vert5);
	vertices.push_back(vert6);
	vertices.push_back(vert6);
	vertices.push_back(vert2);
	vertices.push_back(vert1);
	vertices.push_back(vert1);
	vertices.push_back(vert1);*/

	//--------
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);
	//back
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(4);
	//left
	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(7);
	indices.push_back(7);
	indices.push_back(3);
	indices.push_back(0);
	//right
	indices.push_back(1);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(2);
	indices.push_back(1);
	//top
	indices.push_back(3);
	indices.push_back(7);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(2);
	indices.push_back(3);
	//bottom
	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(5);
	indices.push_back(1);
	indices.push_back(0);

	simpleCube->updateIndices(indices);
	simpleCube->updateVertices(vertices);

	/*glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);*/
	//glDrawElements(GL_LINE_STRIP, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);
	//glBindVertexArray(0);

	colorShader->use();
	colorShader->setMat4("projection", projection);
	colorShader->setMat4("view", view);
	colorShader->setMat4("model", mat4(1.0));
	colorShader->setVec4("aColor", vec4(0.4, 0.3f, 0.2, 0.5f));
	simpleCube->draw(colorShader->get(), GL_TRIANGLES);
}

void GameWorld::drawMouse() {
	double screenX, screenY;
	glfwGetCursorPos(window, &screenX, &screenY);

	screenX = screenX / (SCR_WIDTH / 2.0f) - 1.0f;
	screenY = screenY / (SCR_HEIGHT / 2.0f) - 1.0f;
	screenY *= -1;//inverse Y coord
	vec3 min = vec3(screenX - 0.05, screenY - 0.05, 0.0f);
	vec3 max = vec3(screenX + 0.05f, screenY + 0.05f, 0.0f);
	Vertex vert1, vert2, vert3, vert4;
	vector<Vertex> vertices;

	vert1.Position = vec3(min.x, max.y, min.z);
	vert2.Position = vec3(max.x, max.y, min.z);
	vert3.Position = vec3(max.x, min.y, min.z);
	vert4.Position = vec3(min.x, min.y, min.z);
	vert1.TexCoords = vec2(0.0f, 0.0f);
	vert2.TexCoords = vec2(1.0f, 0.0f);
	vert3.TexCoords = vec2(1.0f, 1.0f);
	vert4.TexCoords = vec2(0.0f, 1.0f);
	vertices.push_back(vert1);
	vertices.push_back(vert2);
	vertices.push_back(vert3);
	vertices.push_back(vert4);

	mouseModel->updateVertices(vertices);
	mouseShader->use();
	mouseModel->draw(mouseShader->get(), GL_TRIANGLES);
}

void drawMouse2(double screenX, double screenY) {
	/*screenX = screenX / SCR_WIDTH/2.0f - 1.0f;
	screenY = screenY / SCR_HEIGHT/2.0f - 1.0f;
	screenY *= -1;//inverse Y coord
	vec3 min = vec3(screenX-0.05, screenY-0.05, 0.0f);
	vec3 max = vec3(screenX+0.05f, screenY+0.05f, 0.0f);
	Vertex vert1, vert2, vert3, vert4;
	Vertex vert5, vert6, vert7, vert8;
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	vert1.Position = vec3(min.x, max.y, min.z);
	vert2.Position = vec3(max.x, max.y, min.z);
	vert3.Position = vec3(max.x, min.y, min.z);
	vert4.Position = vec3(min.x, min.y, min.z);
	//vert1.TexCoords = vec2(min.x, min.y);
	//vert2.TexCoords = vec2(min.x + width, min.y);
	//vert3.TexCoords = vec2(min.x + width, min.y + height);
	//vert4.TexCoords = vec2(min.x, min.y + height);
	vert1.TexCoords = vec2(0.0f, 0.0f);
	vert2.TexCoords = vec2(1.0f, 0.0f);
	vert3.TexCoords = vec2(1.0f, 1.0f);
	vert4.TexCoords = vec2(0.0f, 1.0f);
	//front
	vertices.push_back(vert1);
	vertices.push_back(vert2);
	vertices.push_back(vert3);
	vertices.push_back(vert4);

	//--------
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(2); indices.push_back(3); indices.push_back(0);


	SimpleModel* simpleModel = new SimpleModel(vertices, indices, true);
	simpleModel->updateIndices(indices);
	Shader* shader = new Shader("vsMouseShader.glsl", "fsMouseShader.glsl", SHADERCOMPLEXITY_SIMPLE);
	shader->use();

	// bind textures
	int i = 0;
	unsigned int textureId = TextureFromFile("cursor_target.png", "models", false);
	glActiveTexture(GL_TEXTURE0+i); // active before binding
	glUniform1i(glGetUniformLocation(shader->getId(), "texture_diffuse1"), i);
	// and finally bind the texture
	glBindTexture(GL_TEXTURE_2D, textureId);
	// draw mesh
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);
	simpleModel->draw(shader->get(), GL_TRIANGLES);

	glActiveTexture(GL_TEXTURE0);*/
}

void GameWorld::drawText(string text, double screenX, double screenY) {

	Vertex vert1, vert2, vert3, vert4;
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	//screenX = 400;
	//screenY = 300;
	//text = "ABCDEFGHIJKLMNOPRSTUVWXYZ\nabcdefghijklmnoprstuvxyz";
	/*text = "";
	string currLife = std::to_string(player->getCurrLife());
	string maxLife = std::to_string(player->getMaxLife());
	string slash = "/";
	text = currLife + slash + maxLife;*/
	double fontSizeX = 20.0;
	double fontSizeY = 23.0;
	double screenFontSizeX = ((fontSizeX) / (SCR_WIDTH / 2.0f) - 1.0f) + 1;//end-start
	double screenFontSizeY = ((fontSizeY) / (SCR_HEIGHT / 2.0f) - 1.0f) + 1;//end-start


	double textOriginX = screenX / (SCR_WIDTH / 2.0f) - 1.0f;
	double textOriginY = screenY / (SCR_HEIGHT / 2.0f) - 1.0f;
	double originScreenX = screenX;
	double originScreenY = screenY;
	double letterSizeX = 32.0;
	double letterSizeY = 32.0;
	double textureLetterSizeX = 32.0 / 512.0;// ((letterSizeX) / (SCR_WIDTH / 2.0f) - 1.0f) + 1;
	double textureLetterSizeY = 32.0 / 256.0;// ((letterSizeY) / (SCR_HEIGHT / 2.0f) - 1.0f) + 1;

	double textCurrentX = textOriginX;
	double textCurrentY = textOriginY;

	for (char c : text) {
		if (c == '\n') {
			screenY -= fontSizeY;
			textCurrentY = screenY / (SCR_HEIGHT / 2.0f) - 1.0f;
			screenX = originScreenX;
			textCurrentX = screenX / (SCR_WIDTH / 2.0f) - 1.0f;
			continue;
		}
		int charCode = (int)c;

		//calculate vertex positions
		vec3 min = vec3(textCurrentX, textCurrentY, 0.0f);
		vec3 max = vec3(textCurrentX + screenFontSizeX, textCurrentY + screenFontSizeY, 0.0f);
		vert1.Position = vec3(min.x, min.y, min.z);
		vert2.Position = vec3(max.x, min.y, min.z);
		vert3.Position = vec3(max.x, max.y, min.z);
		vert4.Position = vec3(min.x, max.y, min.z);

		//calculate texture positions
	/*	int line = (charCode - 32.0) / 16.0;
		int col = (charCode - (32.0 + line * 16.0));
		double minCoordX = col*32.0/512.0;
		double minCoordY = line*32.0/256.0;
		//cout << c << ": " << line << ", " << col << "::" << minCoordX << ", " << minCoordY << endl;

		vert4.TexCoords = vec2(minCoordX, minCoordY);
		vert3.TexCoords = vec2(minCoordX + textureLetterSizeX, minCoordY);
		vert2.TexCoords = vec2(minCoordX + textureLetterSizeX, minCoordY + textureLetterSizeY);
		vert1.TexCoords = vec2(minCoordX, minCoordY + textureLetterSizeY);*/


		int line = (charCode - 32.0) / 22.0;
		int col = (charCode - (32.0 + line * 22.0));
		double minCoordX = col*23.0 / 512.0;
		double minCoordY = line*32.0 / 256.0;
		textureLetterSizeX = 23.0 / 512.0;
		textureLetterSizeY = 32.0 / 256.0;
		vert4.TexCoords = vec2(minCoordX, minCoordY);
		vert3.TexCoords = vec2(minCoordX + textureLetterSizeX, minCoordY);
		vert2.TexCoords = vec2(minCoordX + textureLetterSizeX, minCoordY + textureLetterSizeY);
		vert1.TexCoords = vec2(minCoordX, minCoordY + textureLetterSizeY);

		//add indices info
		int nrVertices = vertices.size();
		indices.push_back(nrVertices + 0);
		indices.push_back(nrVertices + 1);
		indices.push_back(nrVertices + 2);
		indices.push_back(nrVertices + 2);
		indices.push_back(nrVertices + 3);
		indices.push_back(nrVertices + 0);

		//add vertices info
		vertices.push_back(vert1);
		vertices.push_back(vert2);
		vertices.push_back(vert3);
		vertices.push_back(vert4);

		
		//iterate word to draw next letter
		screenX += fontSizeX*0.65;
		textCurrentX = screenX / (SCR_WIDTH / 2.0f) - 1.0f;
	}

	/*vertices.clear();
	indices.clear();
	Vertex v5, v6, v7, v8, v9;
	v1.Position = vec3(0.0, 0.0, 0.0);
	v2.Position = vec3(0.3, 0.0, 0.0);
	v3.Position = vec3(0.3, 0.3, 0.0);
	v4.Position = vec3(0.0, .3, 0.0);
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(0);
	v5.Position = vec3(0.5, 0.5, 0.0);
	v6.Position = vec3(0.8, 0.5, 0.0);
	v7.Position = vec3(0.8, .8, 0.0);
	v8.Position = vec3(0.5, .8, 0.0);
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(4);

	//vertices
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);
	vertices.push_back(v5);
	vertices.push_back(v6);
	vertices.push_back(v7);
	vertices.push_back(v8);*/

	SimpleModel* verdanaText = new SimpleModel(vertices, indices, "fonts/VerdanaFont.bmp");

	mouseShader->use();
	verdanaText->draw(mouseShader->get(), GL_TRIANGLES);
}

void GameWorld::initGUIImage(SimpleModel& model, double screenX, double screenY,double width, double height) {
	//screenX = screenY = 0;
	//width = 200;
	//height = 100;
	double startScreenX = screenX / (SCR_WIDTH / 2.0f) - 1.0f;
	double startScreenY = screenY / (SCR_HEIGHT / 2.0f) - 1.0f;
	double endScreenX = (screenX+ width) / (SCR_WIDTH / 2.0f) - 1.0f;
	double endScreenY = (screenY+ height) / (SCR_HEIGHT / 2.0f) - 1.0f;
	double normalizedImgWidth = endScreenX - startScreenX;
	double normalizedImgHeight = endScreenY - startScreenY;
	//startScreenY *= -1;//inverse Y coord
	//endScreenY *= -1;//inverse Y coord
	vec3 min = vec3(startScreenX , startScreenY , 0.0f);
	vec3 max = vec3(startScreenX + normalizedImgWidth, startScreenY + normalizedImgHeight, 0.0f);
	Vertex vert1, vert2, vert3, vert4;
	vector<Vertex> vertices;

	vert1.Position = vec3(min.x, max.y, min.z);
	vert2.Position = vec3(max.x, max.y, min.z);
	vert3.Position = vec3(max.x, min.y, min.z);
	vert4.Position = vec3(min.x, min.y, min.z);
	vert1.TexCoords = vec2(0.0f, 0.0f);
	vert2.TexCoords = vec2(1.0f, 0.0f);
	vert3.TexCoords = vec2(1.0f, 1.0f);
	vert4.TexCoords = vec2(0.0f, 1.0f);
	vertices.push_back(vert1);
	vertices.push_back(vert2);
	vertices.push_back(vert3);
	vertices.push_back(vert4);

	model.updateVertices(vertices);
}

GLuint vao, vbo, ibo;
void GameWorld::drawLines(vector<Vertex> vertices, vector<unsigned int> indices) {

	if (vertices.size()==0 || indices.size()==0) {
		vertices.clear();
		indices.clear();
		Vertex v1, v2, v3, v4;
		v1.Position = vec3(0.0f, 10.0f, 0.0f);
		v2.Position = vec3(10.0f, 10.0f, 10.0f);
		vertices.push_back(v1);
		vertices.push_back(v2);
		indices.push_back(0);
		indices.push_back(1);
	}
	

	//SimpleModel* simpleModel = new SimpleModel(vertices, indices, "");
	//simpleModel->updateIndices(indices);
	//Shader* shader = new Shader("vsLineShader.glsl", "fsLineShader.glsl", SHADERCOMPLEXITY_SIMPLE);
	//shader->use();

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//vertex coord
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	//glDrawArrays(GL_LINES, 0, (GLsizei)vertices.size());
	glDrawElements(GL_LINE_STRIP, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}

int contor = 0;
void GameWorld::draw() {
	projection = perspective(radians(camera->getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 440.0f);
	view = camera->getViewMatrix();
	glClearColor(0.2, 0.75, 0.4, 1.0);

	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	primitiveScreenShader->use();
	primitiveScreenShader->setVec3("color", vec3(0.9, 0.2, 0.2));
	bar->getModel()->draw(primitiveScreenShader->get(), GL_TRIANGLES);

	mouseShader->use();
	textManager->draw(mouseShader->get());
	mainPanel->draw(mouseShader->get(), GL_TRIANGLES);
	playerInfo->draw(mouseShader->get(), GL_TRIANGLES);

	if (selectedEnemy != NULL) {
		mouseShader->use();
		enemyInfo->draw(mouseShader->get(), GL_TRIANGLES);

		string name = selectedEnemy->getName();
		string maxLife = std::to_string(selectedEnemy->getMaxLife());
		string currLife = std::to_string(selectedEnemy->getCurrLife());
		string maxArmor = std::to_string(selectedEnemy->getMaxArmor());
		string currArmor = std::to_string(selectedEnemy->getCurrArmor());
		string text = name + "\n";
		text += "Life: "+ currLife + "/" + maxLife + "\n";
		text += "Armor: "+currArmor + "/" + maxArmor + "\n";
		textManager->updateText("EnemyInfo", text,SCR_WIDTH,SCR_HEIGHT);
	}
	//drawText("abcdefghijklmnoprstuvwABCDEFGHIJKLMNOPRSTUVWXYZ", 10, 500);

	animShader->use();
	for (pair<string, int> pair : animModel->boneIds) {
		int boneIdx = pair.second;
		string boneName = pair.first;
		stringstream ss;
		ss << "boneMatrices[" << boneIdx << "]";
		GLuint bonesUniformIndex = glGetUniformLocation(animShader->getId(), ss.str().c_str());
		mat4 finalMatrix = animModel->boneTransformations.at(boneIdx);
		animShader->setMat4(bonesUniformIndex, finalMatrix);
	}
	animShader->setMat4("projection", projection);
	animShader->setMat4("view", view);
	animShader->setMat4("model",  player->getModel().getRotationMatrix());
	animModel->draw(animShader->get());

	//animatedModel
	//model->setSelected(!model->isSelected());
	/*if (model->isSelected())
		drawWithOutline(model);
	else {
		gridShader->use();
		gridShader->setMat4("projection", projection);
		gridShader->setMat4("view", view);
		gridShader->setMat4("model", model->getMatrix()*model->getRotationMatrix());
	//	model->draw(gridShader->get());
	}*/ 

	//draw lines[test]
//	drawLines();

	//draw mouse
	// drawMouse();

	//draw center line
	/*glLineWidth(8);
	glColor3f(0.3, 0.4, 0.9);
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	Vertex v;
	v.Position = vec3(0.0, -100.0, 0.0f);
	vertices.push_back(v);
	v.Position = vec3(0.0, 100.0, 0.0f);
	vertices.push_back(v);
	indices.push_back(0);
	indices.push_back(1);
	drawLines(vertices,indices);
	glLineWidth(3);
	glColor3f(0.3, 0.4, 0.9);*/
	//centerLine->draw(lineShader->get(), GL_LINES);

	//drawText("Hello there!", 10,10);


	//draw terrain
	gridShader->use();
	gridShader->setMat4("projection", projection);
	gridShader->setMat4("view", view);
	gridShader->setMat4("model", gameMap->getTerrainMatrix());
	gridShader->setFloat("drawGrid", (drawGrid ? 1.0f : 0.0f));//draw grid
	gameMap->getTerrain().draw(gridShader->get());


	//draw enemies
	gridShader->use();
	gridShader->setMat4("projection", projection);
	gridShader->setMat4("view", view);
	for (int a = 0; a < enemies.size(); ++a) {
		Model m = enemies.at(a)->getModel(); //
		if (m.isSelected())
			drawWithOutline(&m);
		else {
			gridShader->use();
			gridShader->setMat4("projection", projection);
			gridShader->setMat4("view", view);
			gridShader->setMat4("model", m.getMatrix());
			m.draw(gridShader->get());
		}
	}

	//other map objects
	gridShader->setFloat("drawGrid", 0.0f);//dont draw grid
	for (Model& m : gameMap->getModels()) {
		if (m.isSelected())
			drawWithOutline(&m);
		else {
			gridShader->use();
			gridShader->setMat4("projection", projection);
			gridShader->setMat4("view", view);
			gridShader->setMat4("model", m.getMatrix());
		//	m.draw(gridShader->get());
		}
	}

	//draw player
	if (player->getModel().isSelected()) {
		vec3 color = vec3(0.9f, 0.2f, 0.1f);
		drawWithOutline(&player->getModel(),color);
	}
	else {
		gridShader->use();
		gridShader->setMat4("projection", projection);
		gridShader->setMat4("view", view);
		gridShader->setMat4("model", player->getModel().getMatrix());
		player->getModel().draw(gridShader->get());
	}

	//drawing ammos
	gridShader->use();
	gridShader->setMat4("projection", projection);
	gridShader->setMat4("view", view);
	ammoManager->draw(gridShader->get());

	//draw raycasting vector
	lineShader->use();
	lineShader->setMat4("projection", projection);
	lineShader->setMat4("view", view);
	lineShader->setMat4("model", mat4(1.0f));
	clickedLine->draw(lineShader->get(), GL_LINE_STRIP);

	//draw path
	lineShader->use();
	lineShader->setMat4("projection", projection);
	lineShader->setMat4("view", view);
	lineShader->setMat4("model", clickedMatrix);
	pathLine->draw(lineShader->get(), GL_LINE_STRIP);


	glfwSwapBuffers(window);
}
//--------------------------------

void func() {
	/*
	//draw terrain
	gridShader->use();
	gridShader->setMat4("projection", projection);
	gridShader->setMat4("view", view);
	gridShader->setMat4("model", gameMap->getTerrainMatrix());
	gridShader->setFloat("drawGrid", (drawGrid ? 1.0f : 0.0f));
	gameMap->draw(gridShader->get());

	//draw player
	//player->getModel().draw(gridShader->get());
	//Model newModel(model->getPath());
	//gridShader->setMat4("model", newModel.getMatrix());
	//newModel.draw(gridShader->get());

	////simple viewport box
	//screenShader->use();
	//float tmp = sinf((float)glfwGetTime());
	//Vertex vert1, vert2, vert3;
	//vert1.Position = vec3(tmp, 0.0f, 0.0f);
	//vert2.Position = vec3(0.0f, 0.5f, 0.0f);
	//vert3.Position = vec3(0.5f, 0.5f, 0.0f);
	//vector<Vertex> vs;
	//vs.push_back(vert1);
	//vs.push_back(vert2);
	//vs.push_back(vert3);
	//screenBox->updateVertices(vs);
	//screenBox->draw(screenShader->get(), GL_TRIANGLES);


	//draw raycasting vector
	lineShader->use();
	lineShader->setMat4("projection", projection);
	lineShader->setMat4("view", view);
	lineShader->setMat4("model", clickedMatrix);
	clickedLine->draw(lineShader->get(), GL_LINES);

	//draw path
	lineShader->setMat4("projection", projection);
	lineShader->setMat4("view", view);
	lineShader->setMat4("model", mat4(1.0f));
	pathLine->draw(lineShader->get(), GL_LINE_STRIP);

	//animations
	animShader->use();
	animShader->setMat4("projection", projection);
	animShader->setMat4("view", view);
	animShader->setMat4("model", model->getMatrix());
	GLint bonesUniformIndex = 0;
	for (pair<string, int> pair : model->m_BoneMapping) {
	int boneIdx = pair.second;
	string boneName = pair.first;
	stringstream ss;
	ss << "bonesMatrices[" << boneIdx << "]";
	bonesUniformIndex = glGetUniformLocation(animShader->getId(), ss.str().c_str());
	mat4 finalMatrix = model->m_BoneInfo[boneIdx].FinalTransformation;
	animShader->setMat4(bonesUniformIndex, finalMatrix);
	}*/
	//	model->draw(animShader->get());

	//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//glStencilFunc(GL_ALWAYS, 1, 0xFF);
	/*
	outlineShader->use();
	outlineShader->setMat4("projection", projection);
	outlineShader->setMat4("view", view);
	outlineShader->setMat4("model", m);
	model->draw(outlineShader->get());*/
}


//static model: Z: -90
//animated model: 