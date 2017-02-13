// a tutorial leurningu from
// https://learnopengl.com/

#if !defined IS_TEST_ENABLED // quick switching between test version and mine

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <SOIL.h>
#include <algorithm>

#include "Shader.h"
#include "InputProcessor.h"
#include "InputAction.h"
#include "InputInfo.h"
#include "Profiler.h"
#include "Camera.h"

//
// Globals
//

GLfloat g_blending = 0.2f;
InputProcessor & g_InputProcessor = InputProcessor::GetInstance();
Profiler g_Profiler;

glm::mat4 view;
glm::mat4 projection;

Camera camera;

//
// Constants
//

const int WIDTH  = 800;
const int HEIGHT = 600;

const GLfloat BLENDING_DELTA =  0.05f;
const GLfloat BLENDING_MIN	 = -1.0f;
const GLfloat BLENDING_MAX   =  1.0f;

const GLfloat ROTATION_DELTA = 0.05f;
const GLfloat CAMERA_SPEED   = 0.005f;

const float NEAR_CLIP = 0.1f;
const float FAR_CLIP  = 100.0f;

//
// Logic
//

void InitInputProcessor(GLFWwindow * window) // reconsider the function
{
	// still looks bad
	std::map<InputInfo, InputAction> init = {
		{
			InputInfo{ GLFW_KEY_UP , 0, GLFW_PRESS, 0 },
			InputAction {
				[&](const double _timeDelta) {
					g_blending += BLENDING_DELTA;
					g_blending = std::min(g_blending, BLENDING_MAX);
				},
				false
			}
		},
		{
			InputInfo{ GLFW_KEY_DOWN , 0, GLFW_PRESS, 0 },
			InputAction {
				[&](const double _timeDelta) {
					g_blending -= BLENDING_DELTA;
					g_blending = std::max(g_blending, BLENDING_MIN);
				},
				false
			}
		},
		{
			InputInfo{ GLFW_KEY_ESCAPE , 0, GLFW_PRESS, 0 },
			InputAction {
				[window](const double _timeDelta) {
					glfwSetWindowShouldClose(window, GL_TRUE);
				},
				false
			}
		},
		{
			InputInfo{ GLFW_KEY_F1 , 0, GLFW_PRESS, 0 },
			InputAction{
				[&](const double _timeDelta) {
					g_Profiler.PrintInfo();
				},
				false
			}
		},
	};

	g_InputProcessor.SetActions(init);
	g_InputProcessor.RegisterInputObject(&camera);
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwSwapInterval(3);
	
	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "YAY", nullptr, nullptr);
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

	InitInputProcessor(window);
	glfwSetKeyCallback(window, g_InputProcessor.ProcessInput);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, g_InputProcessor.ProcessMouseMovement);
	glfwSetScrollCallback(window, g_InputProcessor.ProcessMouseScroll);

	const Shader shader(
		"../Shaders/vertex.vs",
		"../Shaders/fragment.frag"
	);

	const GLfloat vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	  
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	  
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	  
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
	  glm::vec3( 0.0f,  0.0f,  0.0f), 
	  glm::vec3( 2.0f,  5.0f, -15.0f), 
	  glm::vec3(-1.5f, -2.2f, -2.5f),  
	  glm::vec3(-3.8f, -2.0f, -12.3f),  
	  glm::vec3( 2.4f, -0.4f, -3.5f),  
	  glm::vec3(-1.7f,  3.0f, -7.5f),  
	  glm::vec3( 1.3f, -2.0f, -2.5f),  
	  glm::vec3( 1.5f,  2.0f, -2.5f), 
	  glm::vec3( 1.5f,  0.2f, -1.5f), 
	  glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Vertex pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)nullptr);
	glEnableVertexAttribArray(0);

	// texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// create textures
	// texture 1
	GLuint texture1;
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load texture image
	int imgWidth;
	int imgHeight;
	unsigned char * img = SOIL_load_image("../Textures/wall.jpg", &imgWidth, &imgHeight, nullptr, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	SOIL_free_image_data(img);
	glBindTexture(GL_TEXTURE_2D, 0);

	// texture 2
	GLuint texture2;
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load texture image
	img = SOIL_load_image("../Textures/awesomeface.png", &imgWidth, &imgHeight, nullptr, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(img);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		g_Profiler.OnFrameBegin();
		const auto & deltaTime = g_Profiler.GetLastFrameDelta();

		glfwPollEvents();

		g_InputProcessor.DispatchInput(deltaTime);

		const auto time = glfwGetTime();

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		const GLuint shaderId = shader.GetProgramId();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shaderId, "assigned_texture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(shaderId, "assigned_texture2"), 1);

		glUniform1f(glGetUniformLocation(shaderId, "blending"), g_blending);

		view = camera.GetViewMatrix();
		projection = glm::perspective(camera.GetFOV(), (float)WIDTH / (float)HEIGHT, NEAR_CLIP, FAR_CLIP);

		glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		shader.Use();

		glBindVertexArray(VAO);		
		for (const auto & pos : cubePositions)
		{
			glm::mat4 model;
			model = glm::translate(model, pos);
			const auto angle = GLfloat(1.0f * sin(time));
			const auto x = GLfloat(sin(time));
			const auto y = GLfloat(cos(time));
			const auto z = GLfloat(sin(time) * cos(time));
			model = glm::rotate(model, angle, glm::vec3(x, y, z));
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glfwSwapBuffers(window);

		g_Profiler.OnFrameEnd();
	}
	
	// shutdown
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}

#endif // quick switching between test version and mine