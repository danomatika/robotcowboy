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
#include "Global.h"

//--------------------------------------------------------------
Global& Global::instance() {
    static Global * pointerToTheSingletonInstance = new Global;
    return *pointerToTheSingletonInstance;
}

//--------------------------------------------------------------
void Global::setup(const int numOutChannels, const int numInChannels,
				    const int sampleRate, const int ticksPerBuffer) {
	
	loadSettings("ka");
	
	audioEngine.setup(numOutChannels, numInChannels,
					  sampleRate, ticksPerBuffer);
	scriptEngine.setup();
	scriptEngine.bootScript = ofToDataPath("boot.lua", true);
	
	osc.setup();
	midi.setup();
	gui.setup();
}

//--------------------------------------------------------------
void Global::clear() {

	audioEngine.clear();
	scriptEngine.clear();
	
	osc.clear();
	midi.clear();
	gui.clear();
}

//--------------------------------------------------------------
void Global::loadSettings(string path) {
	
	ofxLua lua;
	lua.init();
	
	// load defaults
	if(!lua.doScript("defaults.lua")) {
		return;
	}
	
	// load user settings
//	if(!lua.doScript(path)) {
//		return;
//	}
	
	lua.pushTable("rc");
	lua.pushTable("settings");

	scenePath = lua.getString("scenePath", scenePath);
	logLevel = (ofLogLevel) ((int) lua.getFloat("logLevel", (int) logLevel));

	lua.pushTable("osc");
	osc.sendAddress = lua.getString("sendAddress", osc.sendAddress);
	osc.sendPort = lua.getFloat("sendPort", osc.sendPort);
	osc.receivePort = lua.getFloat("receivePort", osc.receivePort);
	lua.popTable();
	
	lua.pushTable("visual");
	scriptEngine.sendsOscOut = lua.getBool("sendsOut", scriptEngine.sendsOscOut);
	lua.popTable();
	
	lua.pushTable("audio");
	audioEngine.sendsOscOut = lua.getBool("sendsOut", audioEngine.sendsOscOut);
	lua.popTable();
	
	lua.pushTable("midi");
	lua.getStringVector("inputs", midi.inputNames);
	lua.getStringVector("outputs", midi.outputNames);
	lua.popTable();
	
	lua.popTable();
	ofLogNotice() << "Loaded settings:";
	lua.printTable(); // print settings
	
	lua.popAllTables();
}

// PRIVATE

//--------------------------------------------------------------
Global::Global() : scenePath("scenes") {
	logLevel = ofGetLogLevel();
}
