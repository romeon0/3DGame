//#pragma comment(lib, "glew32s.lib")
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include "camera.h"


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
// Camera Attributes
//player

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
	mouseSensitivity(SENSITIVTY), zoom(ZOOM)
{
	this->position = position;
	worldUp = up;
	worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
	yaw = 0.0f;//0, 90
	pitch = 90.0f;
	updateCameraVectors();

	gameFront = worldRight;
	gameUp = worldUp;
	gameRight = glm::vec3(0.0f, 0.0f, 1.0f);
}
// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, 
	float upX, float upY, float upZ, 
	float yaw, float pitch) : 
	front(glm::vec3(0.0f, 0.0f, -1.0f)), 
	movementSpeed(SPEED), 
	mouseSensitivity(SENSITIVTY),
	zoom(ZOOM)
{
	position = glm::vec3(posX, posY, posZ);
	worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
	/*return glm::lookAt(glm::vec3(0.0f, 10.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	up*glm::vec3(5.0f, 3.0f, 5.0f));*/
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void  Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
	/*float velocity = movementSpeed * deltaTime + 0.05f;
	if (direction == FORWARD) {
		if (position.x + velocity > 50) {}
		else
			position += gameFront * velocity;

	}
	if (direction == BACKWARD) {
		if (position.x - velocity < -50) {}
		else
			position -= gameFront * velocity;
	}
	if (direction == LEFT)
		position -= gameRight * velocity;
	if (direction == RIGHT)
		position += gameRight * velocity;*/

	float velocity = 0.05;
	if (direction == FORWARD) {
			position += worldUp * velocity;
	}
	if (direction == BACKWARD) {
		position -= worldUp * velocity;
	}
	if (direction == LEFT)
		position -= worldRight * velocity;
	if (direction == RIGHT)
		position += worldRight * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void  Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainpitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainpitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	// update front, right and up Vectors using the updated Eular angles
	updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void  Camera::processMouseScroll(float yoffset)
{
	//	if (zoom >= 1.0f && zoom <= 90.0f)
	zoom -= yoffset;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	//	if (zoom >= 45.0f)
	//		zoom = 90.0f;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
	// Calculate the new front vector
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(front);

	// Also re-calculate the right and up vector
	right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}

float Camera::getZoom(){ return zoom; }
