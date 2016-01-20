//=====================================================
// ProfileSection.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_ProfileSection__
#define __included_ProfileSection__

#include <vector>
class Console;
class OpenGLRenderer;

enum ProfileCategory{
	Rendering,
	Rendering2,
	Input,
	Physics,
	AI,
	Gameplay,
	PlayerProfile,
	Memory,
	Memory2,
	OtherProfile,
	UnknownProfile,
	NumProfileCategories
};

class ProfileSection{
private:
	double m_startTime;
	ProfileCategory m_category;

	static double s_currentFrameTimesMicroseconds[NumProfileCategories];
	static double s_averageFrameTimesMicroseconds[NumProfileCategories];
	static unsigned int s_totalFrameCount;
	static bool s_useAverageTimes;
	static bool s_renderTimes;

	static Console* s_profilingRenderer;
	
public:
	bool m_isRunning;

	ProfileSection(ProfileCategory category = UnknownProfile);
	~ProfileSection();

	static void FinishProfilingFrame();

	static void StartupProfiling(OpenGLRenderer* renderer);
	static void Render(const OpenGLRenderer* renderer);
	static void Print();
	static void ShutdownProfiling(const OpenGLRenderer* renderer);

	inline static void ToggleUseAverages() { s_useAverageTimes = !s_useAverageTimes; }
	inline static void ToggleRendering() { s_renderTimes = !s_renderTimes; }
	inline static void SetUseAverages(bool useAverages) { s_useAverageTimes = useAverages; }
	inline static void SetRendering(bool doRender) { s_renderTimes = doRender; }

	void Start();
	void Stop();
};
#define PROFILE ProfileSection profileMacro
#define PROFILE_CATEGORY(Category) ProfileSection profileMacro(Category)

#endif