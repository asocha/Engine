//=====================================================
// ProfileSection.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "ProfileSection.hpp"
#include "Engine/Time/Time.hpp"
#include "Engine/Console/Console.hpp"

double ProfileSection::s_currentFrameTimesMicroseconds[NumProfileCategories];
double ProfileSection::s_averageFrameTimesMicroseconds[NumProfileCategories];
unsigned int ProfileSection::s_totalFrameCount = 0;
bool ProfileSection::s_useAverageTimes = true;
bool ProfileSection::s_renderTimes = false;

Console* ProfileSection::s_profilingRenderer = nullptr;

///=====================================================
/// 
///=====================================================
ProfileSection::ProfileSection(ProfileCategory category/* = Unknown*/) :
m_startTime(GetCurrentSeconds()),
m_category(category),
m_isRunning(true){
}

///=====================================================
/// 
///=====================================================
ProfileSection::~ProfileSection(){
	Stop();
}

///=====================================================
/// 
///=====================================================
void ProfileSection::FinishProfilingFrame(){
	double inverseNewFrameCount = 1.0 / (s_totalFrameCount + 1);
	for (int i = 0; i < NumProfileCategories; ++i){
		s_averageFrameTimesMicroseconds[i] = (s_averageFrameTimesMicroseconds[i] * s_totalFrameCount + s_currentFrameTimesMicroseconds[i]) * inverseNewFrameCount;
		s_currentFrameTimesMicroseconds[i] = 0.0;
	}

	++s_totalFrameCount;
}

///=====================================================
/// 
///=====================================================
void ProfileSection::StartupProfiling(OpenGLRenderer* renderer){
	s_profilingRenderer = new Console();
	s_profilingRenderer->Startup(renderer);
	s_profilingRenderer->ToggleCursor();
}

///=====================================================
/// 
///=====================================================
std::string PROFILE_CATEGORY_NAMES[NumProfileCategories]{
	"Rendering",
	"Rendering2",
	"Input",
	"Physics",
	"AI",
	"Gameplay",
	"Player",
	"Memory",
	"Memory2",
	"Other",
	"Unknown"
};

void ProfileSection::Render(const OpenGLRenderer* renderer){
	FATAL_ASSERT(renderer != nullptr);
	if (!s_renderTimes)
		return;

	s_profilingRenderer->ClearText();

	for (int i = 0; i < NumProfileCategories; ++i){
		double time = s_useAverageTimes ? s_averageFrameTimesMicroseconds[i] : s_currentFrameTimesMicroseconds[i];

		if (time != 0.0){
			s_profilingRenderer->PrintText(PROFILE_CATEGORY_NAMES[i] + ": " + std::to_string(time));
		}
	}

	s_profilingRenderer->RenderText(renderer, Font::ARIAL, 32.0f, Vec2(1300.0f, 10.0f));
}

///=====================================================
/// 
///=====================================================
void ProfileSection::Print(){
	for (int i = 0; i < NumProfileCategories; ++i){
		double time = s_useAverageTimes ? s_averageFrameTimesMicroseconds[i] : s_currentFrameTimesMicroseconds[i];

		if (time != 0.0){
			ConsolePrintf("%s: %f\n", PROFILE_CATEGORY_NAMES[i].c_str(), time);
		}
	}
}

///=====================================================
/// 
///=====================================================
void ProfileSection::ShutdownProfiling(const OpenGLRenderer* renderer){
	s_profilingRenderer->Shutdown(renderer);

	delete s_profilingRenderer;
	s_profilingRenderer = nullptr;
}

///=====================================================
/// 
///=====================================================
void ProfileSection::Start(){
	m_startTime = GetCurrentSeconds();
	m_isRunning = true;
}

///=====================================================
/// 
///=====================================================
void ProfileSection::Stop(){
	if (!m_isRunning)
		return;

	double duration = GetCurrentSeconds() - m_startTime;
	s_currentFrameTimesMicroseconds[m_category] += duration * (1000.0 * 1000.0);
	m_isRunning = false;
}
