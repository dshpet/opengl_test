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

glm::mat4 model;
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
			InputInfo{ GLFW_KEY_UP, 0, GLFW_PRESS, 0 },
			InputAction {
				[&](const double _timeDelta) {
					g_blending += BLENDING_DELTA;
					g_blending = std::min(g_blending, BLENDING_MAX);
				},
				false
			}
		},
		{
			InputInfo{ GLFW_KEY_DOWN, 0, GLFW_PRESS, 0 },
			InputAction {
				[&](const double _timeDelta) {
					g_blending -= BLENDING_DELTA;
					g_blending = std::max(g_blending, BLENDING_MIN);
				},
				false
			}
		},
		{
			InputInfo{ GLFW_KEY_ESCAPE, 0, GLFW_PRESS, 0 },
			InputAction {
				[window](const double _timeDelta) {
					glfwSetWindowShouldClose(window, GL_TRUE);
				},
				false
			}
		},
		{
			InputInfo{ GLFW_KEY_F1, 0, GLFW_PRESS, 0 },
			InputAction{
				[&](const double _timeDelta) {
					g_Profiler.PrintInfo();
				},
				false
			}
		},
		{
			InputInfo{ GLFW_KEY_F2, 0, GLFW_PRESS, 0 },
			InputAction{
				[&](const double _timeDelta) {
					g_Profiler.TakeScreenshot();
				},
				false
			}
		}
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

	const Shader lightingShader(
		"../Shaders/lighting.vs",
		"../Shaders/lighting.frag"
	);
	const Shader lampShader(
		"../Shaders/lamp.vs",
		"../Shaders/lamp.frag"
	);

	const GLfloat lightVertices[] = {
      -0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
	  
      -0.5f, -0.5f,  0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,
	  
      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
	  
       0.5f,  0.5f,  0.5f,
       0.5f,  0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
	  
      -0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f, -0.5f, -0.5f,
	  
      -0.5f,  0.5f, -0.5f,
       0.5f,  0.5f, -0.5f,
       0.5f,  0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
	};

	const glm::vec3 lightPosition = glm::vec3(0.0f, 1.0f, 0.0f);
	
	// cube section
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);
	
	// Vertex pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
	// lamp section
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		g_Profiler.OnFrameBegin();
		const auto & deltaTime = g_Profiler.GetLastFrameDelta();

		glfwPollEvents();

		g_InputProcessor.DispatchInput(deltaTime);

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render cube
		lightingShader.Use();
		const GLuint lightingShaderID = lightingShader.GetProgramId();

		GLint objectColorLoc = glGetUniformLocation(lightingShaderID, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(lightingShaderID, "lightColor");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); // Also set light's color (white)

		view = camera.GetViewMatrix();
		projection = glm::perspective(camera.GetFOV(), (float)WIDTH / (float)HEIGHT, NEAR_CLIP, FAR_CLIP);
		model = glm::mat4();
		
		glUniformMatrix4fv(glGetUniformLocation(lightingShaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightingShaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(lightingShaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// render lamp
		lampShader.Use();
		const GLuint lampShaderID = lampShader.GetProgramId();

		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube

		glUniformMatrix4fv(glGetUniformLocation(lampShaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lampShaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(lampShaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

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