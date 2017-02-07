#include "InputProcessor.h"

void InputProcessor::SetAction(const InputInfo _key, std::function<void()> _action)
{
	m_KeyActionMap[_key] = _action;
}

void InputProcessor::SetActions(decltype(m_KeyActionMap) & _init)
{
	m_KeyActionMap = std::move(_init); // is move correct?
}

void InputProcessor::ProcessInput(GLFWwindow * _window, int _key, int _scancode, int _action, int _mode)
{
	const InputInfo info{ _key, _scancode, _action,  _mode};
	auto & intance = GetInstance();

	const auto iter = intance.m_KeyActionMap.find(info);
	if (iter == intance.m_KeyActionMap.end())
		return;

	// invoke function
	iter->second();
}
