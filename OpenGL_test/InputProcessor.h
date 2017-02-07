#pragma once

#include <map>
#include <functional>
#include <GLFW/glfw3.h>

struct InputInfo
{
	int key;
	int scancode;
	int action;
	int mode;

	InputInfo(int _key, int _scancode, int _action, int _mode)
	{
		key = _key;
		scancode = _scancode;
		action = _action;
		mode = _mode;
	}

	bool operator< (const InputInfo &o) const
	{
		return key < o.key;
	}
};

class InputProcessor
{
private:
	std::map<InputInfo, std::function<void()>> m_KeyActionMap;

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
	
	void SetAction(const InputInfo _key, std::function<void()> _action);
	void SetActions(decltype(m_KeyActionMap) & _init);
};

