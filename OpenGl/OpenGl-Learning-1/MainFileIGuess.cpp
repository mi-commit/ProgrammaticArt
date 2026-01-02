//Im learning following https://learnopengl.com/ as my source. 


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



float vertices[] = {
	//pos					//color
	 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,// top right
	 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,// bottom right
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,// bottom left
	-0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 0.0f// top left 
};
unsigned int indices[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

void  setupVertexArrays(unsigned int & VAO, unsigned int &VBO, unsigned int & ElementBuffer) {

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
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

	//called every time user resizes window, so that things function still yay
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//create shader program::
	Shader shader("3.3.shader.vert", "3.3.shader.frag");

	unsigned int VBO;
	unsigned int VAO, ElementBuffer;

	setupVertexArrays(VAO, VBO, ElementBuffer);

	//render loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f),
		glClear(GL_COLOR_BUFFER_BIT);
		float time = glfwGetTime();
		float green = (sin(time) / 2.0f) + 0.5f;
		shader.Use();
		shader.SetFloat4("Color", 0.0f, green, 0.0f, 1.0f);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		processInput(window); 



		glfwPollEvents();
		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	int size = width < height ? width : height;
	std::cout << "WIDTH: " << width << " HEIGHT: " << height << " SIZE: "<< size << std::endl;
	glViewport(0, 0, size, size);
}