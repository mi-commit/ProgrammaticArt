#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	glm::vec3 position = glm::vec3(0, 0, 3);
	glm::vec3 front = glm::vec3(0, 0, -1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 right;

	float Yaw;
	float Pitch;

	float speed = 2.0;
	float sens = 0.1;



	glm::mat4 mat_projection;
	glm::mat4 mat_view;
	const float near_distance = 0.1f,
		far_distance = 100.0f;



	Camera(glm::vec3 _pos, glm::vec3 _front, glm::vec3 _up, float _fov = glm::radians(45.0f), float _aspect = 800.0f / 600.0f) {
		mat_projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, near_distance, far_distance);
		position = _pos;
		front = _front;
		up = _up;
		Yaw = YAW;
		Pitch = PITCH;
		UpdateVectors();

	}
	glm::mat4 get_viewMatrix() {
		mat_view = glm::lookAt(position, position + front, up);

		return mat_view;
	}
	void Move(Camera_Movement direction, float dt) {
		float velocity = speed * dt;
		switch (direction) {
		case FORWARD:
			position += velocity * front;
			break;
		case BACKWARD:
			position -= velocity * front;
			break;
		case LEFT:
			position += velocity * glm::normalize(glm::cross(up, front));
			break;
		case RIGHT:
			position -= velocity * glm::normalize(glm::cross(up, front));
			break;

		case UP:
			position += velocity * up;
			break;
		case DOWN:
			position -= velocity * up;
			break;
		}

	}
	void Turn(float xOffset, float yOffset) {
		xOffset *= sens;
		yOffset *= sens;

		Yaw += xOffset;
		Pitch += yOffset;

		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
		UpdateVectors();

	}
private:
	void UpdateVectors() {
		glm::vec3 _front;
		_front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		_front.y = sin(glm::radians(Pitch));
		_front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front = glm::normalize(_front);

		right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0)));
		up = glm::normalize(glm::cross(right, front));

	}
};

