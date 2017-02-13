#pragma once
#include "InputAction.h"
#include "InputInfo.h"
#include <map>

class IInputable
{
public:
	virtual void ProcessInput(const std::vector<InputInfo> & _keys, const double _timeDelta) = 0; // not really good style
	virtual void ProcessMouse(const double _deltaX, const double _deltaY) = 0;
	virtual void ProcessMouseWheel(const double _deltaX, const double _deltaY) = 0;
};
