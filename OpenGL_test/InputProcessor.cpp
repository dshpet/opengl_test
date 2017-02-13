#include "InputProcessor.h"

void InputProcessor::SetAction(const InputInfo _key, const InputAction & _action)
{
	m_KeyActionMap[_key] = std::move(_action); // is move correct? Maybe its better to copy so memory is continuous
}

void InputProcessor::SetActions(decltype(m_KeyActionMap) & _init)
{
	m_KeyActionMap = std::move(_init); // is move correct?
}

const bool InputProcessor::RegisterInputObject(IInputable * _object)
{
	// maybe check for duplicates
	m_Input.push_back(_object);

	return true;
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

	instance.m_MouseDeltaX = _x - instance.m_MouseX;
	instance.m_MouseDeltaY = instance.m_MouseY - _y; // Reversed since y-coordinates range from bottom to top

	instance.m_MouseDeltaX *= MOUSE_SENSITIVITY;
	instance.m_MouseDeltaY *= MOUSE_SENSITIVITY;

	// move to dispatch
	for (const auto & it : instance.m_Input)
		it->ProcessMouse(instance.m_MouseDeltaX, instance.m_MouseDeltaY);

	instance.m_MouseX = _x;
	instance.m_MouseY = _y;
}

void InputProcessor::ProcessMouseScroll(GLFWwindow * _window, double _x, double _y)
{
	const auto & instance = GetInstance();

	const double deltaX = _x * MOUSE_SENSITIVITY;
	const double deltaY = _y * MOUSE_SENSITIVITY;

	// move to dispatch
	for (const auto & it : instance.m_Input)
		it->ProcessMouseWheel(deltaX, deltaY);
}

void InputProcessor::DispatchInput(const double _timeDelta)
{
	const auto & name = logname();

	for (const auto & it : m_KeyActionMap)
	{
		// todo rename
		const auto & savedInputInfo = it.first;
		const auto & savedAction = it.second;

		auto w = m_keys[savedInputInfo.key];
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

	// temporary hack for getting rid of GLFW_REPEAT
	// todo rethink input system to be more user-friendly
	for (auto & it : m_keys)
		if (it.action == GLFW_REPEAT)
			it.action = GLFW_PRESS;

	for (const auto & it : m_Input)
		it->ProcessInput(m_keys, _timeDelta);
}