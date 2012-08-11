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

	scenePath = lua.readString("scenePath", scenePath);
	logLevel = (ofLogLevel) lua.readUInt("logLevel", (int) logLevel);

	lua.pushTable("osc");
	osc.sendAddress = lua.readString("sendAddress", osc.sendAddress);
	osc.sendPort = lua.readUInt("sendPort", osc.sendPort);
	osc.receivePort = lua.readUInt("receivePort", osc.receivePort);
	lua.popTable();
	
	lua.pushTable("visual");
	scriptEngine.sendsOscOut = lua.readBool("sendsOut", scriptEngine.sendsOscOut);
	lua.popTable();
	
	lua.pushTable("audio");
	audioEngine.sendsOscOut = lua.readBool("sendsOut", audioEngine.sendsOscOut);
	lua.popTable();
	
	lua.pushTable("midi");
	lua.readStringVector("inputs", midi.inputNames);
	lua.readStringVector("outputs", midi.outputNames);
	lua.popTable();
	
	lua.popAllTables();
}

// PRIVATE

//--------------------------------------------------------------
Global::Global() : scenePath("scenes") {
	logLevel = ofGetLogLevel();
}
