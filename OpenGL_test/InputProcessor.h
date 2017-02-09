#pragma once

#include <map>
#include <GLFW/glfw3.h>
#include "InputAction.h"
#include "InputInfo.h"

class InputProcessor
{
private:
	std::map<InputInfo, InputAction> m_KeyActionMap;
	InputInfo m_keys[1024];

private:
	InputProcessor() = default;
	~InputProcessor() = default;
	
	InputProcessor(InputProcessor const &) = default; // prevent copies
	void operator= (InputProcessor const &) { throw std::bad_exception(); }; // prevent assignments

public:
	// happy little singletone CoolStoryBob
	static InputProcessor& GetInstance()
	{
		static InputProcessor instance;
		return instance;
	};

	// hack for usage in GLFW callback
	static void ProcessInput(GLFWwindow * _window, int _key, int _scancode, int _action, int _mode);
	void DispatchInput();
	
	void SetAction(const InputInfo _key, const InputAction & _action);
	void SetActions(decltype(m_KeyActionMap) & _init);

	// not really needed and should be some redone in a more convinient way
	static const char * const logname() { return "[InputProcessor]"; }
};

