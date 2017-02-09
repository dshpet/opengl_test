#include "Profiler.h"
#include <ratio>

auto Profiler::GetCurrentTime() const
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
	m_FrameDelta = GetCurrentTime();
}

void Profiler::OnFrameEnd()
{
	const auto now = GetCurrentTime();
	m_FrameDelta = now - m_FrameDelta;

	const double delta = m_FrameDelta.count();

	m_AverageFrameTime = (m_AverageFrameTime + delta) / 2;
	m_MaxFrameTime = std::fmax(m_MaxFrameTime, delta);
	m_MinFrameTime = std::fmin(m_MinFrameTime, delta);
}

void Profiler::PrintInfo() const
{
	printf("Average frame time: %f \n Max frame time: %f \n Min frame time: %f \n", m_AverageFrameTime, m_MaxFrameTime, m_MinFrameTime);
}
