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

void InputProcessor::DispatchInput()
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
			savedAction.Execute();

			// erase from the queue
			if (!savedAction.IsRepeated())
				m_keys[savedInputInfo.key] = InputInfo();
		}
	}
}
