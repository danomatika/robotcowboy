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

#include "ofxOsc.h"

#include "AudioEngine.h"
#include "ScriptEngine.h"

// a singleton for global variables
class Global {

    public:

        /// singleton data access
		/// returns a reference to itself
		/// creates a new object on the first call
        static Global& instance();

		/// \section Objects

		ofxOscSender oscSender;       	///< global osc sender
        ofxOscReceiver oscReceiver;		///< global osc receiver
		
		AudioEngine audioEngine;		///< pd engine
		ScriptEngine scriptEngine;		///< lua engine
		
		/// \section Variables
		
		string oscSendAddress;
		int oscSendPort;
		int oscReceivePort;
		
		bool visualSendsOut;
		bool audioSendsOut;
		
		/// \section Functions
		
		/// send osc messages
		void sendOscFromAudio(ofxOscMessage& msg);
		void sendOscFromScript(ofxOscMessage& msg);

    private:
        
        // hide all the constructors, copy functions here
        Global(); 							// singleton constructor
        Global(Global const&);    			// not defined, not copyable
        Global& operator = (Global const&);	// not defined, not assignable
};
