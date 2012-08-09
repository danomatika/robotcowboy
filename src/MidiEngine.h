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

#include "ofxMidi.h"

class MidiEngine : public ofxMidiListener, public ofxMidiConnectionListener {

	public:
	
		MidiEngine();
	
		bool setup();
		void clear() {}
		
		// sending
		void sendNoteOn(const int channel, const int pitch, const int velocity);
		void sendControlChange(const int channel, const int controller, const int value);
		void sendProgramChange(const int channel, const int value);
		void sendPitchBend(const int channel, const int value);
		void sendAftertouch(const int channel, const int value);
		void sendPolyAftertouch(const int channel, const int pitch, const int value);
		
		// midi message callback
		void newMidiMessage(ofxMidiMessage& msg);
		
		// midi device (dis)connection event callbacks
		void midiInputAdded(string name, bool isNetwork);
		void midiInputRemoved(string name, bool isNetwork);
		
		void midiOutputAdded(string nam, bool isNetwork);
		void midiOutputRemoved(string name, bool isNetwork);
		
		vector<ofxMidiIn*> inputs;
		vector<ofxMidiOut*> outputs;
};
