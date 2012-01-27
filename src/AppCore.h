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
#pragma once

#include "ofMain.h"

#include "ofxPd.h"
#include "ofxLua.h"
#include "ofxOsc.h"

#include "PdReceiver.h"

class AppCore : public ofxLuaListener {

	public:

		AppCore();

		// main
		void setup(const int numOutChannels, const int numInChannels,
				   const int sampleRate, const int ticksPerBuffer);
		void update();
		void draw();
        void exit();
		
		// input callbacks
		void keyPressed(int key);
		void mousePressed(int x, int y, int button);
		
		// audio callbacks
		void audioReceived(float * input, int bufferSize, int nChannels);
		void audioRequested(float * output, int bufferSize, int nChannels);
		
		// lua error callback
		void errorReceived(const std::string& msg);
		
		// script control
		
		// exit, reinit the lua state, and reload the current script
		void reloadScript();
		
		/// send an osc message to the lua script
		/// calls the oscReceived function
		void scriptOscReceived(ofxOscMessage& msg);
		
		/// send an osc message to pd
		/// sends to OSC_IN
		void pdSendOsc(ofxOscMessage& msg);
		
		ofxPd pd;
		ofxLua lua;
		string currentScript;
		
		ofxOscSender& sender;
		ofxOscReceiver& receiver;
		
		PdReceiver pdReceiver;
};
