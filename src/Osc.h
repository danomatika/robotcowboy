/*
 * Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/robotcowboy for documentation
 *
 */
#pragma once

#include "ofxOsc.h"

class Osc {

	public:
	
		Osc();
		
		void setup();
		void clear() {}
		
		/// handle any incoming messages
		void update();
		
		/// send osc messages
		void sendOscFromAudio(ofxOscMessage& msg);
		void sendOscFromScript(ofxOscMessage& msg);
		void sendOscFromScript(ofxOscBundle& bundle);
		
		ofxOscSender sender;
        ofxOscReceiver receiver;
		
		string sendAddress;
		int sendPort;
		int receivePort;
};
