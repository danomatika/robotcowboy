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
#include "Osc.h"
#include "Midi.h"
#include "Gui.h"

class AppCore;

// a singleton for global variables
class Global {

    public:

        /// singleton data access
		/// returns a reference to itself
		/// creates a new object on the first call
        static Global& instance();

		/// \section Objects

		AppCore *core;					///< global app pointer
		
		AudioEngine audioEngine;		///< pd wrapper
		ScriptEngine scriptEngine;		///< lua wrapper
		
		Osc osc;			///< osc wrapper
		Midi midi;			///< midi wrapper
		Gui gui;			///< gui wrapper
		
		/// \section Variables
		
		string scenePath;	///< absolute path to scene root
		
		ofLogLevel logLevel;
		
		/// \section Functions
		
		/// load global settings from a lua script
		void loadSettings(string path);
		
		/// setup objects
		void setup(const int numOutChannels, const int numInChannels,
				   const int sampleRate, const int ticksPerBuffer);
		
		/// clear all objects
		void clear();

    private:
        
        // hide all the constructors, copy functions here
        Global(); 							// singleton constructor
        Global(Global const&);    			// not defined, not copyable
        Global& operator = (Global const&);	// not defined, not assignable
};
