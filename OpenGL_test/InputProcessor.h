#pragma once

#include <map>
#include <vector>
#include <GLFW/glfw3.h>

#include "InputAction.h"
#include "InputInfo.h"
#include "IInputable.h"

class InputProcessor
{
private:
	static constexpr double MOUSE_SENSITIVITY = 0.05;

private:
	std::map<InputInfo, InputAction> m_KeyActionMap;
	std::vector<InputInfo> m_keys;

	std::vector<IInputable *> m_Input;

	GLfloat m_MouseX;
	GLfloat m_MouseY;

	GLfloat m_MouseDeltaX;
	GLfloat m_MouseDeltaY;

	bool m_isFirstFocus = true;
	
private:
	InputProcessor()
		:
		m_KeyActionMap(),
		m_keys(1024),
		m_Input(),
		m_MouseX(),
		m_MouseY(),
		m_MouseDeltaX(),
		m_MouseDeltaY()
	{
		// Empty
	};
	~InputProcessor() = default;
	
	InputProcessor(InputProcessor const &) = default; // prevent copies
	void operator= (InputProcessor const &) { throw std::bad_exception(); }; // prevent assignments

public:
	// happy little singletone CoolStoryBob
	static InputProcessor & GetInstance()
	{
		static InputProcessor instance;
		return instance;
	};

	// hack for usage in GLFW callback
	static void ProcessInput(GLFWwindow * _window, int _key, int _scancode, int _action, int _mode);
	void DispatchInput(const double _timeDelta);

	static void ProcessMouseMovement(GLFWwindow * _window, double _x, double _y);
	static void ProcessMouseScroll(GLFWwindow * _window, double _x, double _y);
	
	void SetAction(const InputInfo _key, const InputAction & _action);
	void SetActions(decltype(m_KeyActionMap) & _init);
	const bool RegisterInputObject(IInputable * _object);

	// not really needed and should be some redone in a more convinient way
	static const char * const logname() { return "[InputProcessor]"; }
};

