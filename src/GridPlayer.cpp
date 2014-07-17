/*
 *  GridPlayer.cpp
 *  midiGridPlayer
 *
 *  Created by Andrew N Robertson on 17/07/2014.
 *  Copyright 2014 QMUL. All rights reserved.
 *
 */

#include "GridPlayer.h"

Poco::Timestamp::TimeDiff hdelay = 500000; // delay between 2 events, in microseconds
Poco::Timestamp::TimeDiff nextTime = 0;    // time when next event occurs (calculated in seqTimerFunc)

int beatticks = 500;

//bug schedule next tick


GridPlayer::GridPlayer()
{	
	std::cout << "midipoco  called" << std::endl;
	stopwatch.start();
	tickCounter = 0;
	midiPlaying = false;
	
	pitchZones = 7;
	rhythmZones = 10;//rhythm deviders
	
	for (int i = 0; i < pitchZones; i++)
		noteMap.push_back(i);
	
	noteMap[0] = 0;
	noteMap[1] = 2;
	noteMap[2] = 3;
	noteMap[3] = 5;
	noteMap[4] = 7;
	noteMap[5] = 9;
	noteMap[6] = 10;
	//noteMap[7] = 12;
	
	noteIndex = 0;
	reverseMode = false;
	hopperMode = false;
	
//	scheduledIndex = 0;
	
	midiOut.listPorts();
	midiOut.openPort(2);
	
	lastPitch = 0;

	quantisation = beatticks;
	


	
}

GridPlayer::~GridPlayer(){
	midiOut.sendNoteOff(1, lastPitch, 127);
}

//void seqTimerFunc(Poco::Timestamp::TimeDiff curTime);
void GridPlayer::seqTimerFunc(Poco::Timestamp::TimeDiff curTime)
// Events that are regularly sent are processed here
{
	/*
	 //classic delay script
	 if(curTime >= nextTime) {
	 nextTime += hdelay;
	 std::cout <<  (long)(curTime / 1000) <<  " .. " << (long)(nextTime / 1000) << " ticks " << tickCounter << std::endl;
	 // Execute real messages (midi etc) here ...		
	 }
	 */
	if (midiPlaying)
		updateGridPlayer();
}



void GridPlayer::updateGridPlayer(){
	tickCounter++;
	if (tickCounter % beatticks == 0)
		std::cout << "GridPlayer: BANG " << tickCounter << std::endl;
	
	checkNote(tickCounter);
	
	
	//	if (scheduledIndex < scheduledEvents->size())
	//		std::cout << "ticks " << tickCounter << "  scheduled event at " << (*scheduledEvents)[scheduledIndex].ticks << std::endl;
	/*
	while (scheduledIndex < scheduledEvents->size() && tickCounter >= (*scheduledEvents)[scheduledIndex].ticks){
		printf("note fire tick %i note %i\n", tickCounter, (*scheduledEvents)[scheduledIndex].pitch);
		midiOut.sendNoteOff(1, lastPitch, 127);
		midiOut.sendNoteOn(1, (*scheduledEvents)[scheduledIndex].pitch, 127);
		printf("note off %i note %i\n", tickCounter, lastPitch);
		lastPitch = (*scheduledEvents)[scheduledIndex].pitch;
		scheduledIndex++;
	}
	midiOut.sendNoteOn(1, (*scheduledEvents)[scheduledIndex].pitch, 127);
	*/
}

void GridPlayer::setRhythm(int zone){
	
	rhythmZone = zone;
	switch (rhythmZone){
		case 0:
			quantisation = beatticks;
			break;
		case 1:
			quantisation = beatticks * (2.0/3.0);
			break;
		case 2:
			quantisation = beatticks * 0.5;
			break;	
		case 3:
			quantisation = beatticks * (1.0/3.0);
			break;
		case 4:
			quantisation = beatticks * 0.25;
			break;
		case 5:
			quantisation = beatticks * (1.0/6.0);
			break;
		case 6:
			quantisation = beatticks * (1.0/8.0);
			break;
		case 7:
			quantisation = beatticks * (1.0/16);
			break;
		case 8:
			quantisation = beatticks * (1.0/32.0);
			break;
		case 9:
			quantisation = beatticks * (1.0/64.0);
			break;	
			
	}
}

void GridPlayer::setPitch(int pitchHeight){
	
	noteIndex = pitchHeight-1;
	if (noteIndex < 0)
		noteIndex += pitchZones;
	
}

void GridPlayer::newBeat(int tempoMillis){
	//offset = tickCounter;
}

void GridPlayer::checkNote(int tickpoint){
	//std::cout << "check " << tickpoint << "quant " << quantisation << std::endl;
	if (tickpoint % quantisation == 0)
		nextNote();
}

void GridPlayer::nextNote(){
	//send note off
	midiOut.sendNoteOff(1, lastPitch, 127);
	//set next note point and pitch
	
	
	if (!reverseMode){
		if (hopperMode)
			++noteIndex;
		
		if (++noteIndex >= pitchZones)
			noteIndex -= pitchZones;
	} else {
		if (hopperMode)
			--noteIndex;
	
		if (--noteIndex < 0)
			noteIndex += pitchZones;
	}
		
	lastPitch = noteMap[noteIndex]+48;
	
	midiOut.sendNoteOn(1, lastPitch, 127);
	
	//std::cout << "note out [" << noteIndex << "] " << lastPitch << std::endl;
}
