#include "GameTimer.h"
#include "PrecureModelDisplayerException.h"
#include <windows.h>

GameTimer::GameTimer() : _startTime(0), _pauseTime(0), _pauseLength(0), _preTime(0), _curTime(0), _running(false), _active(false)
{
	//Initialize PerformanceFrequency
	bool Result = QueryPerformanceFrequency((LARGE_INTEGER*)&_frequency);
	CheckResult(Result, PMD_FailedQueryPerformanceFrequency());
}


void GameTimer::Start()
{
	bool Result = false;
	//Check Status
	if (_active = false)
	{
		//Active the Timer
		//Initialize Start Time Count
		Result = QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);
		CheckResult(Result, PMD_FailedQueryPerformanceCounter());

		//Initialize Other Time Counts
		_preTime = _startTime;
		_curTime = _startTime;

		//Set Flag
		_running = true;
		_active = true;
	}
	else if (_running = false)
	{
		//Resume from pausing
		//Accumulate the length of pausing in count
		__int64 resuming = 0;
		Result = QueryPerformanceCounter((LARGE_INTEGER*)&resuming);
		CheckResult(Result, PMD_FailedQueryPerformanceCounter());
		__int64 plength = resuming - _pauseTime;
		if (plength >= 0)
		{
			//Ensure it is positive
			_pauseLength += plength;
		}
		//Reset Pause Time Count
		_pauseTime = 0;

		//Set Flag 
		_running = true;
	}

	//Others: Do nothing
}

void GameTimer::Reset()
{
	//Reset everything
	_startTime = 0;
	_pauseTime = 0;
	_pauseLength = 0;
	_preTime = 0;
	_curTime = 0;
	_running = false;
	_active = false;
}

void GameTimer::Pause()
{
	if (_active && _running)
	{
		//Runing
		//Record Pause Time Count
		bool Result = QueryPerformanceCounter((LARGE_INTEGER*)&_pauseTime);
		CheckResult(Result, PMD_FailedQueryPerformanceCounter());

		//Set Flag
		_running = false;
	}
}

void GameTimer::Tick()
{
	if (_active && _running)
	{
		//Running
		//Swap
		_preTime = _curTime;
		//Record the Current Frame Time Count
		bool Result = QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
		CheckResult(Result, PMD_FailedQueryPerformanceCounter());
	}
}

double GameTimer::getDeltaTime() const
{
	if (_active && _running)
	{
		//Calculate the difference between 2 frames
		__int64 difference = _curTime - _preTime;
		if (difference < 0)
		{
			return 0.0;
		}
		//Calculate the time interval in ms
		double result = difference / _frequency;
		result *= 1000.0;

		return result;
	}
	//else
	return 0.0;
}

double GameTimer::getTotalTime() const
{
	if (_active && _running)
	{
		//Record Current Time Count
		__int64 currentTime;
		bool Result = QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
		CheckResult(Result, PMD_FailedQueryPerformanceCounter());
		
		//Calculate the Time Difference
		__int64 difference;
		difference = currentTime - _startTime - _pauseLength;
		if (difference < 0)
		{
			return 0.0;
		}
		//Calculate the time interval in sec.
		double result = difference / _frequency;

		return result;
	}
	//else
	return 0.0;
}



