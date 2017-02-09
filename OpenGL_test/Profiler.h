#pragma once

#include <math.h>
#include <chrono>
#include <limits>

// probably should be singleton
class Profiler
{
private:
	double m_AverageFrameTime;
	double m_MaxFrameTime;
	double m_MinFrameTime;

	std::chrono::milliseconds m_FrameDelta;
public:

	Profiler()	:
		m_AverageFrameTime(0),
		m_FrameDelta(),
		m_MaxFrameTime(std::numeric_limits<decltype(m_MaxFrameTime)>::min()), 
		m_MinFrameTime(std::numeric_limits<decltype(m_MinFrameTime)>::max())
	{
		// Empty
	};

	~Profiler() = default;

public:
	void OnFrameBegin();
	void OnFrameEnd();

	void PrintInfo() const;

private:
	double GetAverageFPS() { return 1000.0f / m_AverageFrameTime; };
	auto GetCurrentTime() const;
};