#include "InputProcessor.h"

void InputProcessor::SetAction(const int _key, std::function<void()> _action)
{
	m_KeyActionMap[_key] = _action;
}

void InputProcessor::ProcessInput(GLFWwindow * _window, int _key, int _scancode, int _action, int _mode)
{
	auto & intance = GetInstance();

	if (intance.m_KeyActionMap.find(_key) == intance.m_KeyActionMap.end())
		return;

	intance.m_KeyActionMap[_key]();
}

void InputProcessor::mouseButtonPressed()
{
	auto fuck = 0;
	return;
}
