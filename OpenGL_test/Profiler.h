#pragma once

#include <math.h>
#include <chrono>
#include <limits>
#include <assert.h>

// probably should be singleton
class Profiler
{
private:
	static constexpr auto FRAME_SMOOTHING_COEFFICIENT = 0.88;

private:
	double m_AverageFrameTime;
	double m_MaxFrameTime;
	double m_MinFrameTime;
	std::chrono::milliseconds m_FrameDelta;
	size_t m_FramesElapsedCount;

	std::chrono::milliseconds m_FrameBeginTime;

public:
	Profiler() :
		m_AverageFrameTime(0),
		m_FrameDelta(),
		m_FrameBeginTime(),
		m_MaxFrameTime(std::numeric_limits<decltype(m_MaxFrameTime)>::min()),
		m_MinFrameTime(std::numeric_limits<decltype(m_MinFrameTime)>::max()),
		m_FramesElapsedCount(0)
	{
		static_assert(
			FRAME_SMOOTHING_COEFFICIENT >= 0.0 && FRAME_SMOOTHING_COEFFICIENT <= 1.0, 
			"FRAME_SMOOTHING_COEFFICIENT represents weight of new delta time measurmenets in average timing. Must be [0.0, 1.0]"
		);
	};

	~Profiler() = default;

public:
	void OnFrameBegin();
	void OnFrameEnd();

	void PrintInfo() const;

	const double GetAverageFrameTime() const;
	const double GetLastFrameDelta() const;

	// Utility
	void TakeScreenshot() const;

private:
	const double GetAverageFPS() const { return 1000.0 / m_AverageFrameTime; };
	const auto GetCurrentTime() const;
};