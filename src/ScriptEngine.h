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

#include "ofMain.h"
#include "ofxLua.h"

#include "TouchEvent.h"

class ofxOscMessage;

class ScriptEngine : private ofxLuaListener {

	public:
	
		ScriptEngine();
	
		bool setup();
		void clear();
		
		/// clears the current lua state
		void clearScript();
		
		/// load a new script
		/// clears the current lua state
		bool loadScript(string script);
		
		/// exit, reinit the lua state, and reload the current script
		bool reloadScript();
		
		/// send an osc message to the lua script
		/// calls the oscReceived lua function
		void sendOsc(ofxOscMessage& msg);
		
		/// touch events
		void touchDown(ofTouchEventArgs &touch);
		void touchMoved(ofTouchEventArgs &touch);
		void touchUp(ofTouchEventArgs &touch);
		void touchDoubleTap(ofTouchEventArgs &touch);
		void touchCancelled(ofTouchEventArgs &touch);
		
		/// receives a print from the lua console
		void print(const string& message);
		void write(const string& message);

		/// did a script error ocurr? if so, grab the message
		bool errorOcurred();
		string getErrorMessage();
		
		ofxLua lua;
		bool sendsOscOut;
		string bootScript; ///< absolute path
		
	private:
	
		/// lua error callback
		void errorReceived(string& msg);

		string currentScript; ///< absolute path to current script

		string writeBuffer; ///< print() & write() buffer for building messages
		TouchEvent touchEvent; ///< to use as a conversion reference
};
