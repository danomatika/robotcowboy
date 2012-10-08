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
	
	dataPath = ofToDataPath("", true);
	
	// load defaults
	loadSettings("ka");//docsPath+"/settings.lua");
	
	audioEngine.setup(numOutChannels, numInChannels,
					  sampleRate, ticksPerBuffer);
	
	scriptEngine.setup();
	scriptEngine.bootScript = ofToDataPath("boot.lua", true);
	
	osc.setup();
	midi.setup();
	physics.setup();
	gui.setup();
	
	// get absolute paths
	
	if(!ofFilePath::isAbsolute(scenePath)) {
		scenePath = ofToDataPath(scenePath+"/", true);
	}
}

//--------------------------------------------------------------
void Global::clear() {

	audioEngine.clear();
	scriptEngine.clear();
	
	osc.clear();
	midi.clear();
	physics.clear();
	gui.clear();
}

//--------------------------------------------------------------
void Global::loadSettings(string path) {
	
	ofxLua lua;
	lua.init();
	
	// load defaults
	if(!lua.doScript(dataPath+"/defaults.lua")) {
		return;
	}
	
	// load user settings on top, if any	
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

//--------------------------------------------------------------
void Global::resetGraphics() {
	ofSetupGraphicDefaults();
	ofSetBackgroundAuto(true);
	ofSetVerticalSync(true);
	ofBackground(100, 100, 100);
	ofSetFrameRate(60);
	ofShowCursor();
}

// PRIVATE

//--------------------------------------------------------------
Global::Global() : dataPath("data"), scenePath("scenes") {
	logLevel = ofGetLogLevel();
}
