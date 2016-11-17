#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

// shaders
const char * VertexShader =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"out vec4 pos;\n"
"void main()\n"
"{\n"
"	pos = vec4(position.x, position.y, position.z, 1.0);\n"
"	gl_Position = pos;\n"
"}"
;

const char * FragmentShader =
"#version 330 core\n"
"in vec4 pos;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"	color = vec4(pos.x, pos.y, pos.z, 1.0);\n"
"}"
;

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE &&
		action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void DrawTriangle()
{

}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	GLFWwindow * window = glfwCreateWindow(800, 600, "YAY", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "NO WINDOW FOR OYU" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Cant init GLEW" << std::endl;
		return -1;
	}

	int width = -1;
	int height = -1;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSetKeyCallback(window, KeyCallback);

	GLfloat triangleVerticies[] = {
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,

		0.6f, -0.4f, 0.0f,
		0.9f, -0.4f, 0.0f,
		0.9f, 0.0f, 0.0f
	};
	GLfloat verticies[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
	
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VertexShader, nullptr);
	glCompileShader(vertexShader);
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar info[512];
		glGetShaderInfoLog(vertexShader, sizeof(info), nullptr, info);
		std::cout << "Shader compilation failed: " << info << std::endl;
	}

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentShader, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar info[512];
		glGetShaderInfoLog(fragmentShader, sizeof(info), nullptr, info);
		std::cout << "Shader compilation failed: " << info << std::endl;
	}

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar info[512];
		glGetShaderInfoLog(shaderProgram, sizeof(info), nullptr, info);
		std::cout << "Shader linker failed: " << info << std::endl;
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	
	glfwTerminate();
	return 0;
}