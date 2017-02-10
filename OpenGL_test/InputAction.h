#pragma once

#include <functional>

class InputAction
{
//
// Members
//
private:
	std::function<void(double)> m_Action;
	bool m_isRepeated;

//
// Construction
//
public:
	InputAction() = default;
	
	explicit InputAction(
		decltype (m_Action) _action, 
		bool _isRepeated
	)	:
		m_Action(_action),
		m_isRepeated(_isRepeated)
	{
		// Empty
	};

//
// Interface
//
public:
	const inline bool IsRepeated() const
	{
		return m_isRepeated;
	}

	void Execute(const double _timeDelta) const
	{
		m_Action(_timeDelta);
	}
};