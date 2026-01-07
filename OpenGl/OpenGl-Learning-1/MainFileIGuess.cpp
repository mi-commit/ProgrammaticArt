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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

float vertices[] = {
	 0.5f,  0.5f,-0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,			// top right
	 0.5f, -0.5f,-0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			// bottom right
	-0.5f, -0.5f,-0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,			// bottom left
	-0.5f,  0.5f,-0.5f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,			// top left 

	//pos					//color				uv
	 0.5f,  0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,			// top right
	 0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,			// bottom right
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,			// bottom left
	-0.5f,  0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f			// top left 
};
unsigned int indices[] = {
	//front face
	0, 1, 3,   // first triangle
	1, 2, 3,    // second triangle
	//bottom
	1, 2, 5,
	2, 5, 6,
	//left
	2, 3 ,7,
	2, 6 ,7,
	//right
	0, 1, 4,
	1, 4, 5,
	//top
	0, 3, 4,
	3, 4, 7,
	//back
	5, 4, 6,
	4, 6, 7
};
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(-1.5f, 0.0f, 0.0),
	glm::vec3(1.5f,  0.0f,  0.0f),
	glm::vec3(0.0f,  1	,  0.0f),
	glm::vec3(0.0f,  3	,  0.0f),
	glm::vec3(0.0f,  4	,  0.0f),
	glm::vec3(0.0f,  2	,  0.0f),
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(0.0f,  0.0f,  0.0f),
};



Camera cam = Camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

float delta_t;


void  setupVertexArrays(unsigned int& VAO, unsigned int& VBO, unsigned int& ElementBuffer) {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &ElementBuffer);

	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//copy index array into a element buffer for opengl
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. then set our vertex attributes pointers
		//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//pos
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}


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
	Shader shader("SHADER/3.3.shader.vert", "SHADER/3.3.shader.frag");

	unsigned int VBO;
	unsigned int VAO, ElementBuffer;

	setupVertexArrays(VAO, VBO, ElementBuffer);

	shader.Use();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	shader.SetInt("ourTexture1", 0);
	shader.SetInt("ourTexture2", 1);





	glm::mat4 mat_Model = glm::mat4(1.0f);

	shader.SetMatrix4x4("model",		mat_Model);
	shader.SetMatrix4x4("view",			cam.mat_view);
	shader.SetMatrix4x4("projection",	cam.mat_projection);




	float lastMeasureTime = glfwGetTime();
	int frameCount = 0;
	//render loop
	while (!glfwWindowShouldClose(window)) {
		float time = glfwGetTime();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);

		shader.SetMatrix4x4("view", cam.get_viewMatrix());

		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		for (int i = 0; i < 1000; i++) {
			glm::mat4 trans = glm::mat4(1.0f);
			trans = glm::translate(trans, cubePositions[i % 10]);
			trans = glm::rotate(trans, time * (i + 1), glm::vec3(i * .4 + 7.1 + time, i * .4 - .1, .1 * i));
			shader.SetMatrix4x4("model", trans);
			glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		}


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