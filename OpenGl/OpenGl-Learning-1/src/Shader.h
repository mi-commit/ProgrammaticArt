#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // for opengl headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

class Shader {
public:
	unsigned int ID;
	Shader(Camera* _cam, const char* vertexPath, const char* fragmentPath);
	void Use() {
		glUseProgram(ID);
		SetMatrix4x4("view",		cam->get_viewMatrix());
		SetMatrix4x4("projection",	cam->mat_projection);


	}
	//utility functions for uniforms
	void SetBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void SetInt(const std::string& name, int  value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);

	}
	void SetFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void SetFloat4(const std::string& name, float x, float y, float z, float w) {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	void SetMatrix4x4(const std::string& name, glm::mat4 matrix) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void SetVec3(const std::string& name, float x, float y, float z) {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void SetVec3(const std::string& name, glm::vec3 vec) {
		glUniform3f(glGetUniformLocation(ID, name.c_str()),vec.x, vec.y, vec.z);
	}

private:
	Camera* cam;
	void Compile(unsigned int shader, const char* source) {
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" <<
				infoLog << std::endl;
		}
	}
};



Shader::Shader(Camera* _cam, const char* vertexPath, const char* fragmentPath) {
	cam = _cam;
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//enable throwing exeptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vertexPath);  //open files
		fShaderFile.open(fragmentPath);//open files

		std::stringstream vShaderStream, fShaderStream;

		//read buffer contents to streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode	=	vShaderStream.str();
		fragmentCode =	fShaderStream.str();
	}
	catch (std:: ifstream:: failure& e){
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::cout << "Compiling VertexShader\n";
	Compile(vertexShader, vShaderCode);
	std::cout << "Compiling FragmentShader\n";
	Compile(fragmentShader, fShaderCode);

	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glDeleteShader(vertexShader); glDeleteShader(vertexShader);
}






#endif