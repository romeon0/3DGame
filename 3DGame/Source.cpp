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
#include <fstream>
#include <random>
#include <vector>
#include "shader.h"
#include "camera.h"
#include "model.h"
using namespace glm;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void setGrid(Shader gridShader, Model terrain);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera
Camera camera(glm::vec3(-50.0f, 32.0f, 0.0f));
double lastX = SCR_WIDTH / 2.0;
double lastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Model terrain, player;
Model tree;
Model* trees;
vec3* treesCoords;
int TREE_COUNT = 0;
vector<Model> objects;
glm::mat4 projection, view;
bool drawGrid = true;

int main()
{ 
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		system("pause");
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (glewInit())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		system("pause");
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	cout << "Compiling shaders...\n";
	Shader ourShader("vsTextureShader.glsl", "fsTextureShader.glsl");
	Shader gridShader("vsGridShader.glsl", "fsGridShader.glsl");
	cout << "Shaders compiled.\n";

	// load models
	// -----------
	ifstream is("modelPath.txt");
	string path;
	string path2;
	string path3;
	is >> path;
	is >> path2;
	is >> path3;
	is.close();


	std::mt19937_64 rng;
	// initialize the random number generator with time-dependent seed
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	rng.seed(ss);
	// initialize a uniform distribution between 0 and 1
	std::uniform_real_distribution<double> unif(0, 1);
	// ready to generate random numbers

	cout << "Loading models...\n";
	terrain.extractData(path);
	player.extractData(path2);
	trees = new Model[TREE_COUNT];
	treesCoords = new vec3[TREE_COUNT];
	int tmp = -1;
	for (int a = 0; a < TREE_COUNT; ++a) {
		//	trees[a].extractData(path3);
		treesCoords[a] = vec3(tmp*rand() % 50, 0, tmp*rand() % 40);
		tmp *= -1;
		//	objects.insert(objects.end(), trees[a]);
	}
	tree.extractData(path3);
	objects.insert(objects.end(), terrain);
	cout << "Models loaded.\n";

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// render loop
	// -----------
	//ourModel.translate(0.0f, 0.0f, 0.0f); // translate it down so it's at the center of the scene
	//*ourShader.setMat4("model", ourModel.getMatrix());
	//player.translate(16.0f, 0.0f, 0.0f);
	//ourModel2.scale(0.99f, 0.99f, 0.99f);

	player.translate(1.0, 1.0, 1.0);

	cout << "GameLoop started.\n";
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = (double)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		srand((uint)currentFrame);

		// input
		// -----
		//	processInput(window);
		// render
		// ------
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		gridShader.use();

		// view/projection transformations
		projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 140.0f);
		view = camera.getViewMatrix();
		gridShader.setMat4("projection", projection);
		gridShader.setMat4("view", view);
		gridShader.setFloat("drawGrid", (drawGrid ? 1.0f : 0.0f));

		// render the loaded model
		gridShader.setMat4("model", terrain.getMatrix());
		terrain.Draw(gridShader);
		gridShader.setMat4("model", player.getMatrix());
		player.Draw(ourShader);
		for (int a = 0; a < TREE_COUNT; ++a) {
			gridShader.setMat4("model", glm::translate(player.getMatrix(), treesCoords[a]));
			tree.Draw(gridShader);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	cout << "GameLoop finished.\n";

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
bool isCollide(vec3 pos, vec3 volume) {
	//cout << "---------------meshes\n";
	//cout << "Player: " 
	//	<< pos.x << ", " 
	//	<< pos.y << ", "
	//	<< pos.z << ", "
	//	<< volume.x << ", "
	//	<< volume.y << ", "
	//	<< volume.z << endl;
	//for (Model model : objects) {
	//	for (int a = 0; a < model.meshes.size(); ++a) {
	//		Mesh mesh = model.meshes[a];
	//		vec3 position = mesh.getCoords();
	//		vec3 param1 = mesh.getVolume();
	//		float x1 = position.x;
	//		float y1 = position.y;
	//		float z1 = position.z;
	//		float x2 = pos.x;
	//		float y2 = pos.y;
	//		float z2 = pos.z;
	//		cout << "Mesh " << a << ": "
	//			<< position.x << ", "
	//			<< position.y << ", "
	//			<< position.z << "::"
	//			<< param1.x << ", "
	//			<< param1.y << ", "
	//			<< param1.z << endl;
	//		if (
	//		    (x1 <= x2+volume.x && x1 + param1.x>= x2) &&
	//			(y1 <= y2 + volume.y && y1 + param1.y >= y2) &&
	//			(z1 <= z2 + volume.z && z1 + param1.z >= z2) ) return true;
	//	}
	//}
	//cout << "END------------meshes\n";
	return false;
}
void show() {
	//first object(map)
	glm::mat4 matrix = terrain.getMatrix();
	vec3 param1 = terrain.getVolume();//divide width and height at 2
	vec3 position = matrix*vec4(1.0f, 1.0f, 1.0f, 1.0f); //map
														 //position -= glm::vec3(param1.x, -param1.y, 0);//make coordinate origin to left-up corner(object1)
														 //second object(cube)
	glm::mat4 matrix2 = player.getMatrix();
	vec3 param2 = player.getVolume();//divide width and height at 2
	vec3 position2 = matrix2*vec4(1.0f, 1.0f, 1.0f, 1.0f); //cube
														   //position2 -= glm::vec3(param2.x, -param2.y, 0);//make coordinate origin to left-up corner(object2)

	cout << "Old position1: "
		<< position.x << ", "
		<< position.y << ", "
		<< position.z << endl;
	cout << "Old position2: "
		<< position2.x << ", "
		<< position2.y << ", "
		<< position2.z << endl;

}



void setGrid(Shader gridShader, Model terrain) {
	//verts = terrain.getVertices();
	//inds = terrain.getIndices();

	//unsigned int VBO, EBO;
	//// create buffers/arrays
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	//glBindVertexArray(VAO);
	//// load data into vertex buffers
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//// A great thing about structs is that their memory layout is sequential for all its items.
	//// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	//// again translates to 3/2 floats which translates to a byte array.
	//glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(unsigned int), &inds[0], GL_STATIC_DRAW);
	//// set the vertex attribute pointers
	//// vertex Positions
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//glBindVertexArray(0);
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);

	float pas = 0.5f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		player.translate(0.0, 0.0, -pas);

		/*vec3 vol = player.getVolume();
		vec3 pos = player.getCoords();
		if (isCollide(pos,vol)) {
		cout << "Collided!!!\n";
		}
		else cout << "OK\n";*/
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		player.translate(0.0, 0.0, pas);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		vec3 pos = player.getCoords();
		pos.y += pas;
		vec3 vol = player.getVolume();
		if (isCollide(pos, vol)) {
			cout << "Collided!!!\n";
		}
		else cout << "OK\n";
		player.translate(pas, 0, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		vec3 pos = player.getCoords();
		pos.y -= pas;
		vec3 vol = player.getVolume();
		if (isCollide(pos, vol)) {
			cout << "Collided!!!\n";
		}
		else cout << "OK\n";
		player.translate(-pas, 0, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		vec3 pos = player.getCoords();
		pos.z -= pas;
		vec3 vol = player.getVolume();
		if (isCollide(pos, vol)) {
			cout << "Collided!!!\n";
		}
		else cout << "OK\n";
		player.translate(0, 0, -pas);
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		vec3 pos = player.getCoords();
		pos.z += pas;
		vec3 vol = player.getVolume();
		if (isCollide(pos, vol)) {
			cout << "Collided!!!\n";
		}
		else cout << "OK\n";
		player.translate(0, 0, pas);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		drawGrid = !drawGrid;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (0.0f != 0) {
		cout << "TRUE\n";
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
	float pas = 2.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		player.translate(0.0, 0.0, -pas);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		player.translate(0.0, 0.0, pas);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		player.translate(pas, 0, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		player.translate(-pas, 0, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		player.translate(0, 0, -pas);
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		player.translate(0, 0, pas);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		drawGrid = !drawGrid;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_sizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}