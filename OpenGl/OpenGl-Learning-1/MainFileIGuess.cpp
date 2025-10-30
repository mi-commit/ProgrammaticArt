//Im learning following https://learnopengl.com/ as my source. 


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


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
	//called every time user resizes window, so that things function still yay
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//render loop
	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	std::cout << "WIDTH: " << width << " HEIGHT: " << height << std::endl;
	glViewport(0, 0, width, height);
}