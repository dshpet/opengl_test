#include "Profiler.h"
#include <ratio>

const auto Profiler::GetCurrentTime() const
{
	const auto & timePoint = std::chrono::high_resolution_clock::now().time_since_epoch();

	const auto & timeInSeconds = std::chrono::duration_cast<std::chrono::seconds>(timePoint);
	const auto & timeInMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(timePoint);

	const auto & timeInMillisecondsIntegral = std::chrono::duration_cast<std::chrono::milliseconds>(timePoint);
	const auto & timeInMilliseconds = std::chrono::duration<double, std::milli>(timePoint);

	return timeInMillisecondsIntegral;
}

void Profiler::OnFrameBegin()
{
	m_FrameBeginTime = GetCurrentTime();
}

void Profiler::OnFrameEnd()
{
	const auto now = GetCurrentTime();
	m_FramesElapsedCount++;
	m_FrameDelta = now - m_FrameBeginTime;

	const double delta = m_FrameDelta.count();

	m_AverageFrameTime = (FRAME_SMOOTHING_COEFFICIENT * delta) + ((1.0 - FRAME_SMOOTHING_COEFFICIENT) * m_AverageFrameTime);
	m_MaxFrameTime = std::fmax(m_MaxFrameTime, delta);
	m_MinFrameTime = std::fmin(m_MinFrameTime, delta);
}

void Profiler::PrintInfo() const
{
	printf(
		"Average FPS: %f \n"
		"Average frame time: %f \n"
		"Max frame time: %f \n"
		"Min frame time: %f \n"
		"LastFrameDelta: %f \n"
		"\n",

		GetAverageFPS(),
		m_AverageFrameTime,
		m_MaxFrameTime,
		m_MinFrameTime,
		GetLastFrameDelta()
	);
}

const double Profiler::GetAverageFrameTime() const
{
	return m_AverageFrameTime;
}

const double Profiler::GetLastFrameDelta() const
{
	return m_FrameDelta.count();
}
