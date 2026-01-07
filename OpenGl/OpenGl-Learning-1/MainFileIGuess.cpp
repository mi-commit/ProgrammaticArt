//Im learning following https://learnopengl.com/ as my source. 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "src/Shader.h"
#include "src/Camera.h"
#include "src/Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);




Camera cam = Camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

float delta_t;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(800, 800, "TEST", NULL, NULL);


	if (window == NULL) {
		std::cout << "Failed Creating Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//initializing glad
	//"pass glad the function to load the address of OpenGL function pointers"
	//glfwGetProcAddress defines correct function based on target address
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//called every time user resizes window, so that things function still yay
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);


	//texture
	// TODO:: MOVE TO SOME OTHER CLASS!!
	uint32_t texture, texture2;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// load image
	if (true) {
		int width, height, nrChannels;
		unsigned char* textureData = stbi_load("IMG/wall.jpg", &width, &height, &nrChannels, 0);
		if (textureData) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "failed to load texture" << std::endl;
		}
		stbi_image_free(textureData);
	}
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glEnable(GL_DEPTH_TEST);

	if (true) {
		int width, height, nrChannels;
		unsigned char* textureData = stbi_load("IMG/awesomeface.png", &width, &height, &nrChannels, 0);
		if (textureData) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "failed to load texture" << std::endl;
		}
		stbi_image_free(textureData);
	}
	
	

	//create shader program::
	Shader shader(&cam, "SHADER/3.3.shader.vert", "SHADER/3.3.shader.frag");
	Model model(&shader,vertices, std::size(vertices), indices, 36);
	model.shader->Use();
	model.shader->SetInt("ourTexture1", 0);
	model.shader->SetInt("ourTexture2", 1);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	



	float lastMeasureTime = glfwGetTime();
	int frameCount = 0;



	//render loop
	while (!glfwWindowShouldClose(window)) {
		float time = glfwGetTime();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		model.Rotate(0.1 * delta_t, glm::vec3(0, 1, 0));
		model.scale = glm::vec3(1, sin(time), 1);
		model.Draw();

		processInput(window);
		glfwPollEvents();
		glfwSwapBuffers(window);

		//if (time - lastMeasureTime > 1) {
		//	std::cout << "fps:  " << (frameCount) << std::endl;
		//	frameCount = 0;
		//	lastMeasureTime = time;
		//}
		//else {
		//	frameCount++;
		//}

		delta_t = glfwGetTime()- time;
	}


	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//camera movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.Move(FORWARD, delta_t);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.Move(BACKWARD, delta_t);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.Move(LEFT, delta_t);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.Move(RIGHT, delta_t);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cam.Move(DOWN, delta_t);

	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cam.Move(UP, delta_t);
	}
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	int size = width < height ? width : height;
	std::cout << "WIDTH: " << width << " HEIGHT: " << height << " SIZE: " << size << std::endl;
	glViewport(0, 0, size, size);
	//mat_projection = glm::perspective(glm::radians(90.0f), (float)(size / size), 0.1f, 100.0f);

}
float lastX = 400, lastY = 300;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;
	cam.Turn(xoffset, yoffset);
}