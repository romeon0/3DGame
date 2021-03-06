 #pragma once
#ifndef CAMERA_H
#define CAMERA_H
#pragma comment(lib, "glew32s.lib")
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
private:
	// Camera Attributes
	//player
	glm::vec3 up;
	glm::vec3 right;
	//openGL world
	glm::vec3 worldRight;
	glm::vec3 worldUp;
	//game world
	glm::vec3 gameUp;//(+-)y axis
	glm::vec3 gameRight;//(+-)z axis
	glm::vec3 gameFront;//(+-)x axis
	glm::mat4 modelMatrix;
	// Euler Angles
	float yaw;
	float pitch;
	// Camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;
	glm::vec3 front;
	glm::vec3 position;
	

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors();
public:
	
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH);
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ,
		float upX, float upY, float upZ,
		float yaw, float pitch);

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 getViewMatrix();

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(Camera_Movement direction, float deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(float yoffset);
	float getZoom();
	void translate(float, float y, float z);
	void goTo(float, float y, float z);
	glm::vec3 Camera::getPosition();

	bool operator==(const Camera& c)
	{
		return position==c.position && up==c.up && yaw==c.yaw && pitch==c.pitch;
	}

};
#endif