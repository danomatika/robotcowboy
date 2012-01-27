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

AppCore::AppCore() :
	sender(Global::instance().getOscSender()),
	receiver(Global::instance().getOscReceiver()) {}

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
		
		// give to lua
		luabind::call_function<bool>(lua, "oscReceived", boost::ref(m));
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
void AppCore::print(const std::string& message) {
	cout << message << endl;
}

//--------------------------------------------------------------		
void AppCore::receiveBang(const std::string& dest) {
	cout << "OF: bang " << dest << endl;
}

void AppCore::receiveFloat(const std::string& dest, float value) {
	cout << "OF: float " << dest << ": " << value << endl;
}

void AppCore::receiveSymbol(const std::string& dest, const std::string& symbol) {
	cout << "OF: symbol " << dest << ": " << symbol << endl;
}

void AppCore::receiveList(const std::string& dest, const List& list) {
	cout << "OF: list " << dest << ": " << list.toString() << endl;
}

void AppCore::receiveMessage(const std::string& dest, const std::string& msg, const List& list) {
	cout << "OF: message " << dest << ": " << msg << " " << list.toString() << list.types() << endl;
}

//--------------------------------------------------------------
void AppCore::receiveNoteOn(const int channel, const int pitch, const int velocity) {
	cout << "OF MIDI: note on: " << channel << " " << pitch << " " << velocity << endl;
}

void AppCore::receiveControlChange(const int channel, const int controller, const int value) {
	cout << "OF MIDI: control change: " << channel << " " << controller << " " << value << endl;
}

// note: pgm nums are 1-128 to match pd
void AppCore::receiveProgramChange(const int channel, const int value) {
	cout << "OF MIDI: program change: " << channel << " " << value << endl;
}

void AppCore::receivePitchBend(const int channel, const int value) {
	cout << "OF MIDI: pitch bend: " << channel << " " << value << endl;
}

void AppCore::receiveAftertouch(const int channel, const int value) {
	cout << "OF MIDI: aftertouch: " << channel << " " << value << endl;
}

void AppCore::receivePolyAftertouch(const int channel, const int pitch, const int value) {
	cout << "OF MIDI: poly aftertouch: " << channel << " " << pitch << " " << value << endl;
}

// note: pd adds +2 to the port num, so sending to port 3 in pd to [midiout],
//       shows up at port 1 in ofxPd
void AppCore::receiveMidiByte(const int port, const int byte) {
	cout << "OF MIDI: midi byte: " << port << " " << byte << endl;
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
