#pragma once

#include <glad/glad.h> // for opengl headers
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

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

class Model {
public:
	glm::vec3 position;
	glm::mat4 model_matrix;
	uint32_t VertexArrayElement;

	Shader*	  shader;
	void Draw() {
		shader->Use();

		glBindVertexArray(VertexArrayElement);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
	}

	Model(Shader* _shader) {
		shader = _shader;
		model_matrix = glm::mat4(1.0f);
		shader->Use();
		shader->SetMatrix4x4("model", model_matrix);


		setupVertexArrays(VertexArrayElement);
	}


private:


	void  setupVertexArrays(unsigned int & VertexArrayElement) {

		unsigned int VBO, ElementBuffer;

		glGenVertexArrays(1, &VertexArrayElement);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &ElementBuffer);

		// 1. bind Vertex Array Object
		glBindVertexArray(VertexArrayElement);
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

};