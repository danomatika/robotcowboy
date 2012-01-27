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
#include "externals/Externals.h"
#include "poco/StringTokenizer.h"

AppCore::AppCore() :
	sender(Global::instance().getOscSender()),
	receiver(Global::instance().getOscReceiver()), pdReceiver(*this) {
	Global::instance().setApp(this);
}

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
	if(!pd.init(numOutChannels, numInChannels, sampleRate, ticksPerBuffer)) {
		ofLog(OF_LOG_ERROR, "Could not init pd");
	}
	Externals::setup();
	pd.addReceiver(pdReceiver);
	pd.addMidiReceiver(pdReceiver);
	pd.subscribe("OSC_OUT");
	pd.addToSearchPath("pd");
	pd.start();
    pd.openPatch("pd/test.pd");
	
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
		ofxOscMessage msg;
		receiver.getNextMessage(&msg);
	
		// get the main destination address
		Poco::StringTokenizer tokenizer(msg.getAddress(), "/");
		Poco::StringTokenizer::Iterator iter = tokenizer.begin()+1;
		string dest = (*iter);
		
		// give to lua or pd
		if(dest == "visual") {
			scriptOscReceived(msg);
		}
		else if(dest == "audio") {
			pdSendOsc(msg);
		}
		else {
			ofLogWarning() << "Unhandled osc message: " << msg.getAddress();
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
	lua.clear();
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
void AppCore::scriptOscReceived(ofxOscMessage& msg) {
	if(!lua.isValid())
		return;
	luabind::call_function<bool>(lua, "oscReceived", boost::ref(msg));
}

//--------------------------------------------------------------
void AppCore::pdSendOsc(ofxOscMessage& msg) {
	
	// get the sub destination
	Poco::StringTokenizer tokenizer(msg.getAddress(), "/");
	Poco::StringTokenizer::Iterator iter;
	string subDest;
	for(iter = tokenizer.begin()+1; iter != tokenizer.end(); ++iter) {
		subDest += "/"+(*iter);
	}
	
	// send to pd as a list
	pd.startMessage();
	pd.addSymbol(subDest);
	for(int i = 0; i < msg.getNumArgs(); ++i) {
		switch(msg.getArgType(i)) {
			case OFXOSC_TYPE_INT32:
				pd.addFloat(msg.getArgAsInt32(i));
				break;
			case OFXOSC_TYPE_FLOAT:
				pd.addFloat(msg.getArgAsInt32(i));
				break;
			case OFXOSC_TYPE_STRING:
				pd.addSymbol(msg.getArgAsString(i));
				break;
		}
	}
	pd.finishList("OSC_IN");
}
