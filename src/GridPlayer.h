/*
 *  GridPlayer.h
 *  midiGridPlayer
 *
 *  Created by Andrew N Robertson on 17/07/2014.
 *  Copyright 2014 QMUL. All rights reserved.
 *
 */


#ifndef GRID_PLAYER
#define GRID_PLAYER

#include "Poco/Stopwatch.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include "Poco/Timer.h"

#include <iostream>

using Poco::Stopwatch;
using Poco::Thread;
using Poco::Timer;
using Poco::TimerCallback;

#include "ofxMidiOut.h"
//#include "MidiSender.h"

//#include "MIDIFileLoader.h"

struct ScheduledNote {
	int pitch;
	unsigned long ticktime;
	
};

class GridPlayer
{
public:
	GridPlayer();
	~GridPlayer();
	
	void seqTimerFunc(Poco::Timestamp::TimeDiff curTime);// prototype of my function ??
	
	void onTimer(Timer& timer)
	{
		seqTimerFunc(stopwatch.elapsed()); // function call
	}
	
	void updateGridPlayer();
	
	void setRhythm(int zone);
	void setPitch(int pitchHeight);
	
	void newBeat(int tempoMillis);
	
	void checkNote(int tickpoint);
	void nextNote();
	
	bool midiPlaying;
	
	unsigned long tickCounter;
	int quantisation;
/*	std::vector<noteData>* scheduledEvents;
	int scheduledIndex;
	*/
	std::vector<int> noteMap;
	int noteIndex;
	
	int lastPitch;
	int rhythmZone;
	int rhythmZones;//change this name
	int pitchZones;
	
	ofxMidiOut midiOut;
	
	
	bool reverseMode;
	bool hopperMode;
	
private:
	Stopwatch stopwatch;
};
#endif
