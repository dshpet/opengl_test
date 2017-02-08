#pragma once

struct InputInfo
{
	int key;
	int scancode;
	int action;
	int mode;

	InputInfo() = default;

	InputInfo(int _key, int _scancode, int _action, int _mode)
	{
		key = _key;
		scancode = _scancode;
		action = _action;
		mode = _mode;
	}

	bool operator< (const InputInfo & o) const
	{
		return key < o.key;
	}

	bool operator== (const InputInfo & o) const
	{
		// do not track mode and scancode
		return key == o.key && action == o.action;
	}
};