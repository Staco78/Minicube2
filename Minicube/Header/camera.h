#pragma once

#include <Glm/glm.hpp>
#include <Glm/gtc/matrix_transform.hpp>
#include <Glm/gtc/type_ptr.hpp>

#include "renderer/window.h"

namespace Minicube {

	class Camera {
	public:
		Camera(Window* window);
		glm::mat4 getMatrix();

		void update();
		glm::vec3 getPosition();
		glm::vec3 getFront();
	private:
		Window* mp_window = nullptr;

		glm::vec3 pos = glm::vec3(0.0f, 0.0f, -3.0f);
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

		bool firstMouse = true;

		double lastX, lastY;
		double yaw = -90;
		double pitch = 0;

		float deltaTime = 0.0f; // Time between current frame and last frame
		float lastFrame = 0.0f; // Time of last frame

	};
}