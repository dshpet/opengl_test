#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

class Shader
{
//
// Construction
//
public:
	explicit Shader(
		const GLchar * _vertexPath,
		const GLchar * _fragmentPath
	)
	{
		std::ifstream vertexShaderFile(_vertexPath);
		std::ifstream fragmentShaderFile(_fragmentPath);

		// explicitly enable exceptions throwing
		vertexShaderFile.exceptions(std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::badbit);

		std::stringstream vertexShaderStream;
		std::stringstream fragmentShaderStream;
		try {
			vertexShaderFile.open(_vertexPath);
			fragmentShaderFile.open(_fragmentPath);
			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();

			std::cout << vertexShaderStream.str() << std::endl;
			std::cout << fragmentShaderStream.str() << std::endl;

			vertexShaderFile.close();
			fragmentShaderFile.close();
		}
		catch (std::ifstream::failure e) {
			std::cerr << "Exception opening/reading/closing file" << std::endl << e.what() << std::endl;
		}

		const std::string vertexTempString = vertexShaderStream.str();
		const std::string fragmentTempString = fragmentShaderStream.str();
		const GLchar * vertexCode = vertexTempString.c_str();
		const GLchar * fragmentCode = fragmentTempString.c_str();

		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexCode, nullptr);
		glCompileShader(vertex);

		GLint success;
		GLchar info[512];
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, sizeof(info), nullptr, info);
			std::cout << "CRITICAL FATAL ERROR: VERTEX SHADER COMPILATION FAILED!!!" << std::endl << info << std::endl;
		}

		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentCode, nullptr);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, sizeof(info), nullptr, info);
			std::cout << "CRITICAL FATAL ERROR: FRAGMENT SHADER COMPILATION FAILED!!!" << std::endl << info << std::endl;
		}

		this->m_ProgramId = glCreateProgram();
		glAttachShader(GetProgramId(), vertex);
		glAttachShader(GetProgramId(), fragment);
		glLinkProgram(GetProgramId());

		glGetProgramiv(GetProgramId(), GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(GetProgramId(), sizeof(info), nullptr, info);
			std::cout << "CRITICAL FATAL ERROR: SHADER LINKING FAILED!!!" << std::endl << info << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	};

//
// Interface
//
public:
	void Use() const
	{
		glUseProgram(GetProgramId());
	}

//
// Members
//
private:	
	GLuint m_ProgramId;

// Getters
public:
	// Weird interaction with auto return type deducing
	// Is almost always okay with explicit auto -> decltype.
	// Function has to be on top with auto f() {return x;}
	auto GetProgramId() const -> decltype(m_ProgramId)
	{ 
		return m_ProgramId; 
	};
};