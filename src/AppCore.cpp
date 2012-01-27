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
#include "AppCore.h"

#include "LuaWrapper.h"
#include "poco/StringTokenizer.h"

AppCore::AppCore() :
	sender(Global::instance().getOscSender()),
	receiver(Global::instance().getOscReceiver()),
	pdReceiver(*this) {}

//--------------------------------------------------------------
void AppCore::setup(const int numOutChannels, const int numInChannels,
				    const int sampleRate, const int ticksPerBuffer) {

	ofSetVerticalSync(true);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	
	// setup osc
	sender.setup(Global::instance().oscSendAddress,
				 Global::instance().oscSendPort);
	receiver.setup(Global::instance().oscReceivePort);
	
	// setup pd
//	if(!pd.init(numOutChannels, numInChannels, sampleRate, ticksPerBuffer)) {
//		ofLog(OF_LOG_ERROR, "Could not init pd");
//	}
//	pd.addReceiver(pdReceiver);
//	pd.addMidiReceiver(pdReceiver);
    
	// setup lua
	currentScript = "scripts/tests/oscTest.lua";
	lua.init();
	lua.bind<LuaWrapper>();
	lua.doScript(currentScript);
	lua.scriptSetup();
}

//--------------------------------------------------------------
void AppCore::update() {
	ofBackground(100, 100, 100);
	lua.scriptUpdate();
	
	// check for waiting osc messages
	while(receiver.hasWaitingMessages()) {
		
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
	
		// get first part of destination address
		Poco::StringTokenizer tokenizer(m.getAddress(), "/");
		Poco::StringTokenizer::Iterator iter = tokenizer.begin()+1;
		string dest = (*iter);
		
		// give to lua
		if(dest == "visual") {
			scriptOscReceived(m);
		}
		else if(dest == "audio") {
			
		}
		else {
			cout << "Unhandled osc message: " << m.getAddress() << endl;
		}
	}
}

//--------------------------------------------------------------
void AppCore::draw() {
	lua.scriptDraw();
}

//--------------------------------------------------------------
void AppCore::exit() {
	lua.scriptExit();
}

//--------------------------------------------------------------
void AppCore::keyPressed(int key) {

	switch(key) {
	
		case 'r':
			reloadScript();
			break;
			
		default:
			break;
	}
	
	lua.scriptKeyPressed(key);
}

//--------------------------------------------------------------
void AppCore::mousePressed(int x, int y, int button) {
	lua.scriptMousePressed(x, y, button);
}

//--------------------------------------------------------------
void AppCore::audioReceived(float * input, int bufferSize, int nChannels) {
	pd.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void AppCore::audioRequested(float * output, int bufferSize, int nChannels) {
	pd.audioOut(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void AppCore::errorReceived(const std::string& msg) {
	cout << "got an error: " << msg << endl;
}

//--------------------------------------------------------------
void AppCore::reloadScript() {
	cout << "reloading script" << endl;
	lua.scriptExit();
	lua.init();
	lua.bind<LuaWrapper>();
	lua.doScript(currentScript);
	lua.scriptSetup();
}

//--------------------------------------------------------------
bool AppCore::scriptOscReceived(ofxOscMessage& msg) {
	luabind::call_function<bool>(lua, "oscReceived", boost::ref(msg));
}
