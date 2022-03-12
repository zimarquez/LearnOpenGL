#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include <iostream>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
private:
	glm::vec3 position{ glm::vec3(0.0f, 0.0f, 3.0f) };
	glm::vec3 front{ glm::vec3(0.0f, 0.0f, -1.0f) };
	glm::vec3 up{ glm::vec3(0.0f, 1.0f, 0.0f) };
	float yaw{ -90.0f };
	float pitch{ 0.0f };
	float lastX{ 400 };
	float lastY{ 300 };
	float fieldOfView{ 45.0f };
	bool firstMouse{ true };
	const float moveSpeed{ 2.5f };
	float sensitivity{ 0.1f };


public:

	Camera()
	{

	}

	void move(Camera_Movement direction, const float deltaTime) {
		switch (direction)
		{
			case FORWARD:
				position += front * moveSpeed * deltaTime;
				break;
			case BACKWARD:
				position -= front * moveSpeed * deltaTime;
				break;
			case LEFT:
				position -= glm::normalize(glm::cross(front, up)) * moveSpeed * deltaTime;
				break;
			case RIGHT:
				position += glm::normalize(glm::cross(front, up)) * moveSpeed * deltaTime;
				break;
		}
		//position.y = 0.0f; // for XZ plane movement only
	}

	void moveHorizontal(const float deltaTime)
	{
		position += glm::normalize(glm::cross(front, up)) * moveSpeed * deltaTime;
	}

	void moveVertical(const float deltaTime)
	{
		position += front * moveSpeed * deltaTime;
	}

	void updateView(double xPosition, double yPosition)
	{
		if (firstMouse) {
			lastX = xPosition;
			lastY = yPosition;
			firstMouse = false;
		}

		float xOffset = xPosition - lastX;
		float yOffset = -(yPosition - lastY);
		lastX = xPosition;
		lastY = yPosition;

		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 viewDirection;
		viewDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		viewDirection.y = sin(glm::radians(pitch));
		viewDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(viewDirection);
	}

	void updateZoom(double yOffset)
	{
		fieldOfView -= (float)yOffset;
		if (fieldOfView < 1.0f)
			fieldOfView = 1.0f;
		if (fieldOfView > 90.0f)
			fieldOfView = 90.0f;
	}

	float getZoom()
	{
		return glm::radians(fieldOfView);
	}
	
	glm::vec3 getPosition()
	{
		return position;
	}

	glm::vec3 getFront()
	{
		return front;
	}

	void resetToStartPosition()
	{
		position = glm::vec3(0.0f, 0.0f, 3.0f);
		front = glm::vec3(0.0f, 0.0f, -1.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		yaw = -90.0f;
		pitch = 0.0f;
		fieldOfView = 45.0f;
	}

	glm::mat4 getView()
	{
		return glm::lookAt(position, position + front, up);
	}
};
#endif