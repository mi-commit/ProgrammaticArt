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
uint32_t initTexture(char* path, GLenum format);



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

	glEnable(GL_DEPTH_TEST);



	////texture
	uint32_t tex_diffuse, tex_specular;
	tex_diffuse = initTexture((char*)"IMG/Crate-diffuse.png\0", GL_RGBA);
	tex_specular = initTexture((char*)"IMG/Crate-specular.png\0", GL_RGBA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//create shader program::
	Shader objectShader(&cam, "SHADER/LitObject_1.vert", "SHADER/LitObject_1.frag");
	Shader lightShader(&cam, "SHADER/LightSource_1.vert", "SHADER/LightSource_1.frag");

	meshes::NormalCube.generate_indices();
	Model Object(&objectShader,meshes::NormalCube.vertices, std::size(meshes::NormalCube.vertices), meshes::NormalCube.indices, std::size(meshes::NormalCube.indices));
	Model Light(&lightShader, meshes::NormalCube.vertices, std::size(meshes::NormalCube.vertices), meshes::NormalCube.indices, std::size(meshes::NormalCube.indices));

	Object.scale = glm::vec3(1);
	Light.pos = (glm::vec3(1, 1.0f, 1));
	Light.scale = (glm::vec3(.1));

	objectShader.Use();
	objectShader.SetInt("material.diffuse", 0); // is a texture
	objectShader.SetInt("material.specular", 1); // is a texture

	objectShader.SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
	//objectShader.SetVec3("material.specular", glm::vec3(0));
	objectShader.SetFloat("material.shininess", 32.0f);

	objectShader.SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	objectShader.SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
	objectShader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

	Object.shader->SetVec3("camPos", cam.position.x, cam.position.y, cam.position.z);






	float lastMeasureTime = glfwGetTime();
	int frameCount = 0;
	//render loop
	while (!glfwWindowShouldClose(window)) {
		float time = glfwGetTime();

		glClearColor(0, 0, 0, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Light.pos.x = sin(time);
		//Light.pos.z = cos(time);
		Light.Draw();

		Object.Rotate(delta_t, glm::vec3(1, 0, 0));
		Object.shader->Use();
		Object.shader->SetVec3("light.position", Light.pos.x, Light.pos.y, Light.pos.z);
		Object.shader->SetVec3("camPos", cam.position.x, cam.position.y, cam.position.z);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_specular);
		Object.Draw();

		processInput(window);
		glfwPollEvents();
		glfwSwapBuffers(window);

		if (time - lastMeasureTime > 1) {
			std::cout << "fps:  " << (frameCount) << std::endl;
			frameCount = 0;
			lastMeasureTime = time;
		}
		else {
			frameCount++;
		}

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

uint32_t initTexture(char* path, GLenum format) {
	uint32_t textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// load image
	if (true) {
		int width, height, nrChannels;
		unsigned char* textureData = stbi_load(path, &width, &height, &nrChannels, 0);
		if (textureData) {
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "failed to load texture" << path << std::endl;
		}
		stbi_image_free(textureData);
	}
	return textureID;
}