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
#include "ofxLua.h"

class ofxOscMessage;

class ScriptEngine : public ofxLuaListener {

	public:
	
		ScriptEngine();
	
		bool setup();
		void clear();
		
		/// load a new script
		/// clears the current lua state
		bool loadScript(string script);
		
		/// exit, reinit the lua state, and reload the current script
		bool reloadScript();
		
		/// send an osc message to the lua script
		/// calls the oscReceived lua function
		void sendOsc(ofxOscMessage& msg);
	
		/// lua error callback
		void errorReceived(const std::string& msg);

		ofxLua lua;
		
	private:

		string currentScript;
};
