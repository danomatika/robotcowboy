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
bool Global::setup(const int numOutChannels, const int numInChannels,
				    const int sampleRate, const int ticksPerBuffer) {
	
	// set dataPath here since it will change a scene is loaded
	dataPath = ofToDataPath("", true);
	
	// settings path: ~/.robotcowboy
	#ifndef TARGET_OF_IPHONE
		ofDirectory settingsDir(ofFilePath::join(ofFilePath::getUserHomeDir(), ".robotcowboy"));
	#else
		ofDirectory settingsDir("Documents"); // should be relative to app root
	#endif
	if(!settingsDir.exists()) {
		ofLogNotice() << "Global: settings directory dosen't exist, creating ...";
		if(!settingsDir.create()) {
			ofLogError() << "Global: couldn't create settings directory";
			return false;
		}
	}
	else if(!settingsDir.isDirectory()) {
		ofLogError() << "Global: settings location is not a directory";
		return false;
	}
	settingsPath = settingsDir.path();
	
	// settings file: ~/.robotcowboy/settings.lua
	ofFile settingsFile(ofFilePath::join(settingsDir.path(), "settings.lua"));
	if(!settingsFile.exists()) {
		// copy defaults file
		if(!ofFile::copyFromTo(ofFilePath::join(dataPath, "defaults.lua"), // src
						   settingsFile.path(), // dest
						   false, false)) { // not relative to data, don't overwrite
			ofLogError() << "Global: couldn't copy default settings file";
			return false;
		}
	}
	
	// load settings
	if(!loadSettings(settingsFile.path())) {
		// bail if problem loading settings
		return false;
	}
	
	// documents & scene paths
	#ifndef TARGET_OF_IPHONE
		// set absolute paths from given realitve paths
		if(!ofFilePath::isAbsolute(docsPath)) {
			docsPath = ofFilePath::join(ofFilePath::getUserHomeDir(), docsPath); 
		}
		if(!ofFilePath::isAbsolute(scenePath)) {
			scenePath = ofFilePath::join(docsPath, scenePath);
		}
	#else
		// these are hardcoded on iOS since the dirs will not change
		docsPath = "Documents";
		scenePath = "Documents/scenes";
	#endif
	ofDirectory docsDir(docsPath);
	if(!docsDir.exists()) {
		ofLogNotice() << "Global: document directory doesn't exist, creating ...";
		if(!docsDir.create()) {
			ofLogError() << "Global: couldn't create documents directory";
			return false;
		}
	}
	ofDirectory sceneDir(scenePath);
	if(!sceneDir.exists()) {
		ofLogNotice() << "Global: scene directory doesn't exist, creating ...";
		if(!sceneDir.create()) {
			ofLogError() << "Global: couldn't create scene directory";
			return false;
		}
	}
	
	print();
	
	// setup modules
	audioEngine.setup(numOutChannels, numInChannels,
					  sampleRate, ticksPerBuffer);
	
	scriptEngine.setup();
	scriptEngine.bootScript = ofFilePath::join(dataPath, "boot.lua");
	
	osc.setup();
	midi.setup();
	physics.setup();
	gui.setup();
	
	return true;
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
bool Global::loadSettings(string path) {
	
	ofxLua lua;
	lua.init();
	
	// load defaults
	if(!lua.doScript(path)) {
		setError("Settings file: "+lua.getErrorMessage());
		return false;
	}
	
	lua.pushTable("rc");
	lua.pushTable("settings");
	docsPath = lua.getString("docsPath", docsPath);
	lua.popAllTables();
	
	readSettings(lua);
	
	lua.pushTable("rc");
	ofLogNotice() << "Loaded settings:";
	lua.printTable(); // print settings
	
	lua.popAllTables();
	
	return true;
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

//--------------------------------------------------------------
void Global::print() {
	ofLogNotice() << "Settings:";
	ofLogNotice() << "	dataPath: " << dataPath;
	ofLogNotice() << "	settingsPath: " << settingsPath;
	ofLogNotice() << "	docsPath: " << docsPath;
	ofLogNotice() << "	scenePath: " << scenePath;
	ofLogNotice() << "	logLevel: " << logLevel;
}

// PRIVATE

//--------------------------------------------------------------
bool Global::readSettings(ofxLua& lua) {
	
	lua.pushTable("rc");
	
	lua.pushTable("settings");
	scenePath = lua.getString("scenePath", scenePath);
	
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
	
	logLevel = (ofLogLevel) ((int) lua.getFloat("logLevel", (int) logLevel));
	
	lua.popAllTables();
}

//--------------------------------------------------------------
Global::Global() {
	dataPath = "data";
	settingsPath = ".robotcowboy";
	docsPath = "Documents/robotcowboy";
	scenePath = "scenes";
	logLevel = ofGetLogLevel();
}
