#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GL\glew.h>
#include <iostream>

#include "IInputable.h"

class Camera :
	public IInputable
{
private:
	static constexpr GLfloat YAW			= 0.0f;
	static constexpr GLfloat PITCH			= 0.0f;
	static constexpr GLfloat ZOOM			= 45.0f;
	static constexpr GLfloat SPEED			= 0.005f;
	static constexpr GLfloat ROTATION_SPEED = 1.0f;
	static constexpr GLfloat ZOOMING_SPEED	= 0.5f;

public:
	Camera() :
		m_Position(glm::vec3(-5.0f, 2.0f, -1.0f)),
		m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),
		m_Yaw(YAW),
		m_Pitch(PITCH),
		m_Zoom(ZOOM),
		m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		m_Speed(SPEED),
		m_RotationSpeed(ROTATION_SPEED),
		m_ZoomingSpeed(ZOOMING_SPEED),
		m_WorldUp(m_Up)
	{
		UpdateCameraVectors();
	};
	
	~Camera() = default;

private:
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

	GLfloat m_Yaw;
	GLfloat m_Pitch;
	GLfloat m_Roll;
	GLfloat m_Zoom;

	GLfloat m_Speed;
	GLfloat m_RotationSpeed;
	GLfloat m_ZoomingSpeed;

// IInputable
public:
	virtual void ProcessInput(const std::vector<InputInfo> & _keys, const double _timeDelta) override
	{
		// not the best idea, but if done like this every object
		// can decide what to do based on current input state

		const GLfloat delta = _timeDelta * m_Speed;
		
		const auto & forward	= InputInfo{ GLFW_KEY_W , 0, GLFW_PRESS, 0 };
		const auto & backward	= InputInfo{ GLFW_KEY_S , 0, GLFW_PRESS, 0 };
		const auto & left		= InputInfo{ GLFW_KEY_A , 0, GLFW_PRESS, 0 };
		const auto & right		= InputInfo{ GLFW_KEY_D , 0, GLFW_PRESS, 0 };
		
		for (const auto & it : _keys)
		{
			if (it == forward)
				m_Position += m_Front * delta;			
			if (it == backward)
				m_Position -= m_Front * delta;
			if (it == left)
				m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * delta;
			if (it == right)
				m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * delta;
		}
	}

	virtual void ProcessMouse(const double _deltaX, const double _deltaY) override
	{
		m_Yaw	+= _deltaX * m_RotationSpeed;
		m_Pitch += _deltaY * m_RotationSpeed;

		// prevent gimbal lock
		m_Yaw	= std::fmin(m_Yaw,	  89.0f);
		m_Yaw	= std::fmax(m_Yaw,	 -89.0f);
		m_Pitch = std::fmin(m_Pitch,  89.0f);
		m_Pitch = std::fmax(m_Pitch, -89.0f);

		UpdateCameraVectors();
	}

	virtual void ProcessMouseWheel(const double _deltaX, const double _deltaY) override
	{
		m_Zoom -= _deltaY * m_ZoomingSpeed;

		m_Zoom = std::fmax(m_Zoom, 1.0f);
		m_Zoom = std::fmin(m_Zoom, 45.0f);
	}

public:
	const glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	const GLfloat GetFOV() const
	{
		return m_Zoom;
	}

	const auto GetPosition() const
	{
		return m_Position;
	}

private:
	void UpdateCameraVectors()
	{
		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		
		m_Front = glm::normalize(front);

		// re-calculate the Right and Up vector
		// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  
		m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
	}
};