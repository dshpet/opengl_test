#pragma once

#include <map>
#include <functional>
#include <GLFW/glfw3.h>

class InputProcessor
{
private:
	InputProcessor() = default;
	~InputProcessor() = default;
	
	InputProcessor(InputProcessor const &); // prevent copies
	void operator= (InputProcessor const &); // prevent assignments

public:
	// happy little singletone CoolStoryBob
	static InputProcessor& GetInstance()
	{
		static InputProcessor instance;
		return instance;
	};

	void SetAction(const int _key, std::function<void()> _action);
	static void ProcessInput(GLFWwindow * _window, int _key, int _scancode, int _action, int _mode);

	void mouseButtonPressed();

private:
	std::map<int, std::function<void()>> m_KeyActionMap;
};

