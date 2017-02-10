#include "InputProcessor.h"

void InputProcessor::SetAction(const InputInfo _key, const InputAction & _action)
{
	m_KeyActionMap[_key] = std::move(_action); // is move correct? Maybe its better to copy so memory is continuous
}

void InputProcessor::SetActions(decltype(m_KeyActionMap) & _init)
{
	m_KeyActionMap = std::move(_init); // is move correct?
}

void InputProcessor::ProcessInput(GLFWwindow * _window, int _key, int _scancode, int _action, int _mode)
{
	//const auto & name = logname();
	//printf("%s key: %i, action: %i \n", name, _key, _action);

	const InputInfo info{ _key, _scancode, _action,  _mode};

	auto & instance = GetInstance();

	// track keys
	instance.m_keys[_key] = info;
}

void InputProcessor::ProcessMouseMovement(GLFWwindow * _window, double _x, double _y)
{
	auto & instance = GetInstance();

	if (instance.m_isFirstFocus)
	{
		instance.m_MouseX = _x;
		instance.m_MouseY = _y;
		instance.m_isFirstFocus = false;
	}

	GLfloat xOffset = _x - instance.m_MouseX;
	GLfloat yOffset = instance.m_MouseY - _y; // Reversed since y-coordinates range from bottom to top

	xOffset *= MOUSE_SENSITIVITY;
	yOffset *= MOUSE_SENSITIVITY;

	instance.m_MouseX = _x;
	instance.m_MouseY = _y;

	instance.yaw	+= xOffset;
	instance.pitch  += yOffset;

	// prevent gimbal lock
	instance.yaw = std::fmin(instance.yaw, 89.0f);
	instance.yaw = std::fmax(instance.yaw, -89.0f);
	instance.pitch = std::fmin(instance.pitch, 89.0f);
	instance.pitch = std::fmax(instance.pitch, -89.0f);
}

void InputProcessor::ProcessMouseScroll(GLFWwindow * _window, double _x, double _y)
{
	auto & instance = GetInstance();

	instance.fov -= _y * MOUSE_SENSITIVITY;
	instance.fov = std::fmax(instance.fov, 1.0f);
	instance.fov = std::fmin(instance.fov, 45.0f);
}

void InputProcessor::DispatchInput(const double _timeDelta)
{
	const auto & name = logname();

	for (const auto & it : m_KeyActionMap)
	{
		// todo rename
		const auto & savedInputInfo = it.first;
		const auto & savedAction = it.second;

		const auto & pressedKeyInfo = m_keys[savedInputInfo.key];

		// not in the list
		if (pressedKeyInfo.key != savedInputInfo.key)
			continue;

		// todo think about refactoring
		if (pressedKeyInfo == savedInputInfo ||
			pressedKeyInfo.action == GLFW_REPEAT && savedAction.IsRepeated())
		{
			//printf("%s dispatched     key: %i, action: %i \n", name, savedInputInfo.key, savedInputInfo.action);
			savedAction.Execute(_timeDelta);

			// erase from the queue
			if (!savedAction.IsRepeated())
				m_keys[savedInputInfo.key] = InputInfo();
		}
	}
}