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
		
//		// pd message receiver callbacks
//		void print(const std::string& message);
//		
//		void receiveBang(const std::string& dest);
//		void receiveFloat(const std::string& dest, float value);
//		void receiveSymbol(const std::string& dest, const std::string& symbol);
//		void receiveList(const std::string& dest, const List& list);
//		void receiveMessage(const std::string& dest, const std::string& msg, const List& list);
//		
//        // pd midi receiver callbacks
//		void receiveNoteOn(const int channel, const int pitch, const int velocity);
//		void receiveControlChange(const int channel, const int controller, const int value);
//		void receiveProgramChange(const int channel, const int value);
//		void receivePitchBend(const int channel, const int value);
//		void receiveAftertouch(const int channel, const int value);
//		void receivePolyAftertouch(const int channel, const int pitch, const int value);
//		
//		void receiveMidiByte(const int port, const int byte);
		
		// lua error callback
		void errorReceived(const std::string& msg);
		
		// script control
		
		// exit, reinit the lua state, and reload the current script
		void reloadScript();
		
		bool scriptOscReceived(ofxOscMessage& msg);
		
		ofxPd pd;
		ofxLua lua;
		string currentScript;
		
		ofxOscSender& sender;
		ofxOscReceiver& receiver;
		
		PdReceiver pdReceiver;
};
