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

#include "ofxPd.h"

class ofxOscMessage;

class AudioEngine : public pd::PdReceiver, public pd::PdMidiReceiver {

	public:
	
		AudioEngine();
	
		bool setup(const int numOutChannels, const int numInChannels,
				   const int sampleRate, const int ticksPerBuffer);
		void clear() {}
		
		/// clears the current patch
		void clearPatch();
		
		/// load a new patch
		/// clears the current patch
		bool loadPatch(std::string patch);
		
		/// send an osc message to pd
		/// sends to OSC_IN
		void sendOsc(ofxOscMessage& msg);
	
		// pd message receiver callbacks
		void print(const std::string& message);
		
		void receiveBang(const std::string& dest);
		void receiveFloat(const std::string& dest, float value);
		void receiveSymbol(const std::string& dest, const std::string& symbol);
		void receiveList(const std::string& dest, const pd::List& list);
		void receiveMessage(const std::string& dest, const std::string& msg, const pd::List& list);
		
        // pd midi receiver callbacks
		void receiveNoteOn(const int channel, const int pitch, const int velocity);
		void receiveControlChange(const int channel, const int controller, const int value);
		void receiveProgramChange(const int channel, const int value);
		void receivePitchBend(const int channel, const int value);
		void receiveAftertouch(const int channel, const int value);
		void receivePolyAftertouch(const int channel, const int pitch, const int value);
		
		void receiveMidiByte(const int port, const int byte);
		
		ofxPd pd;
		pd::Patch currentPatch;
		
		bool sendsOscOut;
};
