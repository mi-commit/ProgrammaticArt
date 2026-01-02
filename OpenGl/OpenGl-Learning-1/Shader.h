#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // for opengl headers
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	void Use() {
		glUseProgram(ID);
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
private:
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



Shader::Shader(const char* vertexPath, const char* fragmentPath) {
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

	unsigned int vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	std::cout << "Compiling VertexShader\n";
	Compile(vertex, vShaderCode);
	std::cout << "Compiling FragmentShader\n";
	Compile(fragment, fShaderCode);

}






#endif