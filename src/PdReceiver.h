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

class AppCore;
class ofxOscSender;

class PdReceiver : public pd::PdReceiver, public pd::PdMidiReceiver {

	public:
	
		PdReceiver(AppCore& app);
	
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
		
	private:
	
		AppCore& app;
		ofxOscSender& sender;
};
