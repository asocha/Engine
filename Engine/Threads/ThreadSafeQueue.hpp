//=====================================================
// ThreadSafeQueue.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_ThreadSafeQueue__
#define __included_ThreadSafeQueue__

#include "Engine/Threads/CriticalSection.hpp"
#include <queue>

template <typename T>
class ThreadSafeQueue{
private:
	std::queue<T> m_queue;
	mutable CriticalSection m_criticalSection;

public:
	ThreadSafeQueue() :
		m_queue(),
		m_criticalSection(){
	}

	void Enqueue(const T& item){
		m_criticalSection.Enter();
		m_queue.push(item);
		m_criticalSection.Exit();
	}

	bool Dequeue(T* out_item){
		bool success = false;
		m_criticalSection.Enter();
		if (!m_queue.empty()) {
			*out_item = m_queue.front();
			m_queue.pop();
			success = true;
		}
		m_criticalSection.Exit();
		return success;
	}

	int Size() const{
		size_t size;
		m_criticalSection.Enter();
		size = m_queue.size();
		m_criticalSection.Exit();

		return size;
	}
};

#endif