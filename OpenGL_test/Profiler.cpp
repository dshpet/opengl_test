#include "Profiler.h"
#include <ratio>
#include <string>
#include <GL\glew.h>
#include <thread>

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

void Profiler::TakeScreenshot() const
{
	// a modified solution from http://stackoverflow.com/questions/3191978/how-to-use-glut-opengl-to-render-to-a-file

	static GLubyte * pixels;
	// todo move to settings or somewhere
	const GLsizei width  = 800;
	const GLsizei height = 600;
	const size_t format_nchannels = 3;

	pixels = (GLubyte *)realloc(pixels, format_nchannels * sizeof(GLubyte) * width * height);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	// do not block main loop
	// cant be done absolutely in separate thread because of EGL context
	std::thread writeToFileJob([=]() 
	{
		// i guess compiler can optimize string stuff
		const std::string folder = "./Logs/Screenshots/";
		const std::string name = "Screenshot_";
		const std::string time = std::to_string(GetCurrentTime().count());
		const std::string extension = ".ppm";
		const std::string concate = folder + name + time + extension;

		const char * filename = concate.c_str();

		size_t i, j, k, cur;

		FILE *f = fopen(filename, "w");

		fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);


		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				cur = format_nchannels * ((height - i - 1) * width + j);
				fprintf(f, "%3d %3d %3d ", (pixels)[cur], (pixels)[cur + 1], (pixels)[cur + 2]);
			}

			fprintf(f, "\n");
		}

		fclose(f);
	});

	// some screenshots can be corrupted on shutdown, but i dont care
	writeToFileJob.detach();

	// maybe i should delete pixels data, but its not a problem for now
}