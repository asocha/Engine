//=====================================================
// Time.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Time__
#define __included_Time__


static double g_secondsPerTick;

void InitializeTimer();
double GetCurrentSeconds();
unsigned long GetOpCount();


#endif