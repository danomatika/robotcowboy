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

		AppCore *core;					//< global app pointer
		
		AudioEngine audioEngine;		//< pd wrapper
		ScriptEngine scriptEngine;		//< lua wrapper
		
		Osc osc;			//< osc wrapper
		Midi midi;			//< midi wrapper
		Gui gui;			//< gui wrapper
		Physics physics;	//< box2d wrapper
		
		/// \section Variables
		
		string dataPath;		//< absolute path to app data folder
		string settingsPath;	//< absolute path to user settings folder
		
		string docsPath;		//< absolute path to user documents folder
		string scenePath;		//< absolute path to user scene folder
		
		ofLogLevel logLevel;	//< log level for console
		
		/// \section Functions
		
		/// load settings from the main lua script
		///
		/// this is in ~/.robotcowboy/settings.lua on desktop,
		/// app/Documents/settings.lua on iOS
		bool loadSettings(string path);
		
		/// setup objects
		bool setup(const int numOutChannels, const int numInChannels,
				   const int sampleRate, const int ticksPerBuffer);
		
		/// clear all objects
		void clear();
		
		/// reset the graphics state
		void resetGraphics();
		
		/// \section Errors
		
		bool errorOcurred() {
			return bErrorOcurred;
		}

		string getErrorMessage() {
			return errorMessage;
		}
		
		void setError(string msg) {
			bErrorOcurred = true;
			errorMessage = msg;
		}
		
		void clearError() {
			bErrorOcurred = false;
			errorMessage = "";
		}
		
		/// \section Util
		
		/// print the current settings
		void print();

    private:
    
		bool bErrorOcurred; //< did some sort of error in the app ocurr?
		string errorMessage; ///< the error message
		
		// read settings from a lua instance, everything but the main folder
		bool readSettings(ofxLua& lua);
		
        // hide all the constructors, copy functions here
        Global(); 							// singleton constructor
        Global(Global const&);    			// not defined, not copyable
        Global& operator = (Global const&);	// not defined, not assignable
};
