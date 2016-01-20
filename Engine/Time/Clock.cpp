//=====================================================
// Clock.cpp
// by Andrew Socha
//=====================================================

#include "Clock.hpp"
#include "Engine/Core/Assert.hpp"
#include "Engine/Core/StringTable.hpp"

Clock* s_theMasterClock = nullptr;

///=====================================================
/// 
///=====================================================
Clock::Clock(Clock* parent) :
m_currentTime(0.0),
m_timeScale(1.0f),
m_maximumDeltaSeconds(0.5f),
m_currentDeltaSeconds(0.0f),
m_isPaused(false),
m_parent(parent),
m_children(){
	if (parent == nullptr){
		RECOVERABLE_ASSERT(s_theMasterClock == nullptr);
		s_theMasterClock = this;
	}
	else{
		parent->m_children.push_back(this);
	}
}

///=====================================================
/// 
///=====================================================
Clock::~Clock(){
	if (this != s_theMasterClock)
		DestroyClock();
	else
		s_theMasterClock = nullptr;
}

///=====================================================
/// 
///=====================================================
void Clock::DestroyClock(){
	FATAL_ASSERT(m_parent != nullptr); //never destroy the master clock

	for (std::vector<Clock*>::iterator childIter = m_children.begin(); childIter != m_children.end(); ++childIter){
		(*childIter)->m_parent = m_parent;
		m_parent->m_children.push_back(*childIter);
	}
}

///=====================================================
/// 
///=====================================================
void Clock::AdvanceTime(float deltaSeconds){
	if (m_isPaused){
		m_currentDeltaSeconds = 0.0;
	}
	else{
		m_currentDeltaSeconds = deltaSeconds * m_timeScale;
		if (m_currentDeltaSeconds > m_maximumDeltaSeconds)
			m_currentDeltaSeconds = m_maximumDeltaSeconds;

		m_currentTime += (double)m_currentDeltaSeconds;

		for (std::map<int, Alarm>::const_iterator alarmIter = m_alarms.cbegin(); alarmIter != m_alarms.cend();){
			if (alarmIter->second.m_endTime < m_currentTime){ //trigger alarm
				const Alarm& alarm = alarmIter->second;
				alarm.m_callback(alarm.m_data);
				delete alarm.m_data;
				alarmIter = m_alarms.erase(alarmIter);
			}
			else{
				++alarmIter;
			}
		}
	}

	for (std::vector<Clock*>::const_iterator childIter = m_children.cbegin(); childIter != m_children.cend(); ++childIter){
		(*childIter)->AdvanceTime(m_currentDeltaSeconds);
	}
}

///=====================================================
/// 
///=====================================================
void Clock::SetAlarm(const std::string& name, double durationSeconds, Callback callback, void* newData){
	RECOVERABLE_ASSERT(m_alarms.find(GetStringID(name)) == m_alarms.end());

	Alarm alarm;
	alarm.m_startTime = m_currentTime;
	alarm.m_endTime = m_currentTime + durationSeconds;
	alarm.m_data = newData;
	alarm.m_callback = callback;
	m_alarms.emplace(GetStringID(name), alarm);
}

///=====================================================
/// 
///=====================================================
void Clock::RemoveAlarm(const std::string& name){
	m_alarms.erase(GetStringID(name));
}

///=====================================================
/// 
///=====================================================
double Clock::GetSecondsRemaining(const std::string& name) const{
	FATAL_ASSERT(m_alarms.find(GetStringID(name)) != m_alarms.end());

	const Alarm& alarm = m_alarms.at(GetStringID(name));
	return (alarm.m_endTime - m_currentTime);
}

///=====================================================
/// 
///=====================================================
double Clock::GetSecondsElapsed(const std::string& name) const{
	FATAL_ASSERT(m_alarms.find(GetStringID(name)) != m_alarms.end());

	const Alarm& alarm = m_alarms.at(GetStringID(name));
	return (m_currentTime - alarm.m_startTime);
}

///=====================================================
/// 
///=====================================================
float Clock::GetPercentRemaining(const std::string& name) const{
	FATAL_ASSERT(m_alarms.find(GetStringID(name)) != m_alarms.end());

	const Alarm& alarm = m_alarms.at(GetStringID(name));
	return (float)((alarm.m_endTime - m_currentTime) / (alarm.m_endTime - alarm.m_startTime));
}

///=====================================================
/// 
///=====================================================
float Clock::GetPercentElapsed(const std::string& name) const{
	FATAL_ASSERT(m_alarms.find(GetStringID(name)) != m_alarms.end());

	const Alarm& alarm = m_alarms.at(GetStringID(name));
	return (float)((m_currentTime - alarm.m_startTime) / (alarm.m_endTime - alarm.m_startTime));
}
