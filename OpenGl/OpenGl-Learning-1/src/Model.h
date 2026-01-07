#pragma once

#include <glad/glad.h> // for opengl headers
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include <vector>

class Model {
public:
	glm::vec3 pos = glm::vec3(0);
	glm::quat rot = glm::quat_cast(glm::mat4(1.0f));
	glm::vec3 scale = glm::vec3(1);



	uint32_t VertexArrayElement;

	Shader* shader;
	void Draw() {
		shader->Use();
		shader->SetMatrix4x4("model", get_modelMatrix());

		glBindVertexArray(VertexArrayElement);
		glDrawElements(GL_TRIANGLES, indexCount * sizeof(uint32_t), GL_UNSIGNED_INT, 0);
	}
	glm::mat4 get_modelMatrix() {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::scale(modelMatrix, scale);
		modelMatrix = mat4_cast(normalize(rot)) * modelMatrix;
		modelMatrix = glm::translate(modelMatrix, pos);

		return modelMatrix;
	}



	void Move(glm::vec3 translation) {
		pos += translation;
	}
	void Rotate(float amount, glm::vec3 axis) {
		rot = glm::rotate(rot, amount, axis);
	}
	void Scale(glm::vec3 amount) {
		scale += amount;
	}

	Model(Shader* _shader, float* vertecies, int _vertCount, uint32_t* _indexes, int _indexCount) {
		vertCount = _vertCount;
		verts = new float[_vertCount];
		for (int i = 0; i < _vertCount; i++) {
			verts[i] = vertecies[i];
		}
		indexCount = _indexCount;
		indexes = new uint32_t[_indexCount];
		for (int i = 0; i < _indexCount; i++) {
			indexes[i] = _indexes[i];
		}
		shader = _shader;
		shader->Use();
		shader->SetMatrix4x4("model", get_modelMatrix());
		setupVertexArrays(VertexArrayElement);
	}


private:
	int vertCount;
	float* verts;
	int indexCount;
	uint32_t* indexes;

	void  setupVertexArrays(unsigned int& VertexArrayElement) {

		unsigned int VBO, ElementBuffer;

		glGenVertexArrays(1, &VertexArrayElement);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &ElementBuffer);

		// 1. bind Vertex Array Object
		glBindVertexArray(VertexArrayElement);
		// 2. copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertCount, verts, GL_STATIC_DRAW);

		//copy index array into a element buffer for opengl
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexCount, indexes, GL_STATIC_DRAW);

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

namespace meshes {
	struct {

		float vertices[64] = {
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

		uint32_t indices[36] = {
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
	} cube;

}