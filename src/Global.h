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
#include "Physics.h"
#include "Midi.h"
#include "Gui.h"

class AppCore;

// a singleton for global variables, states, etc
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
		Physics physics;	///< box2d wrapper
		
		/// \section Variables
		
		/// these are set on startup
		string dataPath;	///< absolute path to app data folder
		string docsPath;	///< absolute path to user documents folder
		string scenePath;	///< absolute path to user scene folder
		
		/// these can be changed when loading a settings file
		ofLogLevel logLevel; ///< log level for console
		
		/// \section Functions
		
		/// laod startup settings from a conf lua script
		
		/// load settings from a lua script
		void loadSettings(string path);
		
		/// setup objects
		void setup(const int numOutChannels, const int numInChannels,
				   const int sampleRate, const int ticksPerBuffer);
		
		/// clear all objects
		void clear();
		
		/// reset the graphics state
		void resetGraphics();

    private:
        
        // hide all the constructors, copy functions here
        Global(); 							// singleton constructor
        Global(Global const&);    			// not defined, not copyable
        Global& operator = (Global const&);	// not defined, not assignable
};
