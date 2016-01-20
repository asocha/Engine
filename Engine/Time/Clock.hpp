//=====================================================
// Clock.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Clock__
#define __included_Clock__

#include <vector>
#include <map>

typedef void(*Callback)(const void * const arg);

struct Alarm{
	double m_startTime;
	double m_endTime;
	void* m_data;
	Callback m_callback;
};

class Clock{
private:
	double m_currentTime;
	float m_currentDeltaSeconds;
	std::map<int, Alarm> m_alarms;
	
	Clock* m_parent;
	std::vector<Clock*> m_children;

public:
	float m_timeScale;
	float m_maximumDeltaSeconds;
	bool m_isPaused;

	Clock(Clock* parent);
	~Clock();

	void DestroyClock();

	void AdvanceTime(float deltaSeconds);

	inline double GetCurrentTime() const { return m_currentTime; }
	inline float GetCurrentDeltaSeconds() const { return m_currentDeltaSeconds; }

	void SetAlarm(const std::string& name, double durationSeconds, Callback callback, void* data);
	void RemoveAlarm(const std::string& name);
	double GetSecondsRemaining(const std::string& name) const;
	double GetSecondsElapsed(const std::string& name) const;
	float GetPercentRemaining(const std::string& name) const;
	float GetPercentElapsed(const std::string& name) const;
};
extern Clock* s_theMasterClock;

#endif