#ifndef GAMETIMER_H
#define GAMETIMER_H


#define CheckResult(Result, e) { if(Result != true) {throw e; } }

class GameTimer
{
public:
	GameTimer();	// Constructor
	void Start();	// When game starts
	void Pause();	// When game pauses
	void Reset();	// Reset the Timer
	void Tick();	// Call between each frame
	 
	double getTotalTime() const;	// Get total time (sec.)
	double getDeltaTime() const;	// Get time difference between frame(ms)
private:
	//For Total Time
	__int64 _startTime;		// The time count when it started
	__int64 _pauseTime;		// The time count when it is paused
	__int64 _pauseLength;	// The total time count of pausing
	//For Delta Time
	__int64 _preTime;	// The time count of previous frame
	__int64 _curTime;	// The time count of current frame
	
	__int64 _frequency; // Counts per Second
	bool _running;		// The running status of Game Timer: true => running, false => pausing
	bool _active;		// The status of Game Timer: true => active, false => inactive (need to be started / have been reseted)
};


#endif
