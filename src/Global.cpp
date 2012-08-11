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
	
	// setup osc
	osc.sender.setup(oscSendAddress, oscSendPort);
	osc.receiver.setup(oscReceivePort);
	
	// setup engines
	audioEngine.setup(numOutChannels, numInChannels,
					  sampleRate, ticksPerBuffer);
	scriptEngine.setup();
	scriptEngine.bootScript = ofToDataPath("boot.lua", true);
	
	// wrappers
	midi.setup();
	gui.setup();
}

//--------------------------------------------------------------
void Global::clear() {

	audioEngine.clear();
	scriptEngine.clear();
	
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

	lua.pushTable("osc");
	oscSendAddress = lua.readString("sendAddress", oscSendAddress);
	oscSendPort = lua.readUInt("sendPort", oscSendPort);
	oscReceivePort = lua.readUInt("receivePort", oscReceivePort);
	lua.popTable();
	
	lua.pushTable("visual");
	audioSendsOut = lua.readBool("sendsOut", visualSendsOut);
	lua.popTable();
	
	lua.pushTable("audio");
	audioSendsOut = lua.readBool("sendsOut", audioSendsOut);
	lua.popTable();
	
	lua.pushTable("midi");
	lua.readStringVector("inputs", midi.inputNames);
	lua.readStringVector("outputs", midi.outputNames);
	lua.popTable();
	
	lua.popAllTables();
}

// PRIVATE

//--------------------------------------------------------------
Global::Global() :
	oscSendAddress("127.0.0.1"), oscSendPort(8880), oscReceivePort(9009),
	visualSendsOut(false), audioSendsOut(false), scenePath("scenes")
	{}
