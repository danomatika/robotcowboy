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
void Global::sendOscFromAudio(ofxOscMessage& msg) {
	if(audioSendsOut) {
		oscSender.sendMessage(msg);
	}
	else {
		scriptEngine.sendOsc(msg);
	}
}

//--------------------------------------------------------------
void Global::sendOscFromScript(ofxOscMessage& msg) {
	if(visualSendsOut) {
		oscSender.sendMessage(msg);
	}
	else {
		audioEngine.sendOsc(msg);
	}
}

// PRIVATE

//--------------------------------------------------------------
Global::Global() :
	oscSendAddress("127.0.0.1"), oscSendPort(8880), oscReceivePort(9009),
	visualSendsOut(false), audioSendsOut(false)
	{}
