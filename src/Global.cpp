/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/robotcowboy for documentation
 *
 */
#include "Global.h"

//--------------------------------------------------------------
Global& Global::instance() {
    static Global * pointerToTheSingletonInstance = new Global;
    return *pointerToTheSingletonInstance;
}

//--------------------------------------------------------------
void Global::setup(const int numOutChannels, const int numInChannels,
				    const int sampleRate, const int ticksPerBuffer) {
	
	// setup osc
	osc.sender.setup(oscSendAddress, oscSendPort);
	osc.receiver.setup(oscReceivePort);
	
	// setup engines
	audioEngine.setup(numOutChannels, numInChannels,
					  sampleRate, ticksPerBuffer);
	scriptEngine.setup();
	
	// wrappers
	midi.setup();
	gui.setup();
}

//--------------------------------------------------------------
void Global::clear() {

	audioEngine.clear();
	scriptEngine.clear();
	
	midi.clear();
	gui.clear();
}

// PRIVATE

//--------------------------------------------------------------
Global::Global() :
	oscSendAddress("127.0.0.1"), oscSendPort(8880), oscReceivePort(9009),
	visualSendsOut(false), audioSendsOut(false)
	{}
