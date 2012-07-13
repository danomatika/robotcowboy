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
#include "AudioEngine.h"

#include "ofxOsc.h"

#include "Global.h"
#include "externals/Externals.h"
#include "poco/StringTokenizer.h"

//--------------------------------------------------------------
AudioEngine::AudioEngine() {}

//--------------------------------------------------------------
bool AudioEngine::setup(const int numOutChannels, const int numInChannels,
						const int sampleRate, const int ticksPerBuffer) {

	if(!pd.init(numOutChannels, numInChannels, sampleRate, ticksPerBuffer)) {
		ofLog(OF_LOG_ERROR, "AudioEngine: Could not init pd");
		return false;
	}

	Externals::setup();
	pd.addReceiver(*this);
	pd.addMidiReceiver(*this);
	pd.subscribe("OSC_OUT");
	pd.addToSearchPath("externals");
	pd.addToSearchPath("externals/rj");
	pd.start();
	return true;
}

//--------------------------------------------------------------
void AudioEngine::sendOsc(ofxOscMessage& msg) {
	
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

//--------------------------------------------------------------
void AudioEngine::print(const std::string& message) {
	cout << "PD: " << message << endl;
}

void AudioEngine::receiveBang(const std::string& dest) {
	cout << "OF: bang " << dest << endl;
	
	ofxOscMessage m;
	m.setAddress(dest);
	Global::instance().sendOscFromAudio(m);
}

void AudioEngine::receiveFloat(const std::string& dest, float value) {
	cout << "OF: float " << dest << ": " << value << endl;
}

void AudioEngine::receiveSymbol(const std::string& dest, const std::string& symbol) {
	cout << "OF: symbol " << dest << ": " << symbol << endl;
	
	ofxOscMessage m;
	m.setAddress(symbol);
	Global::instance().sendOscFromAudio(m);
}

void AudioEngine::receiveList(const std::string& dest, const pd::List& list) {
	cout << "OF: list " << dest << ": " << list.toString() << endl;
	
	if(!list.isSymbol(0)) {
		ofLogWarning() << "AudioEngine: Malformed osc message, ignoring" << endl;
		return;
	}
	
	ofxOscMessage m;
	for(int i = 0; i < list.len(); ++i) {
		if(list.isFloat(i))
			m.addFloatArg(list.getFloat(i));
		else
			m.addStringArg(list.getSymbol(i));
	}
	Global::instance().sendOscFromAudio(m);
}

void AudioEngine::receiveMessage(const std::string& dest, const std::string& msg, const pd::List& list) {
	cout << "OF: message " << dest << ": " << msg << " " << list.toString() << list.types() << endl;
	
	ofxOscMessage m;
	m.setAddress(msg);
	for(int i = 0; i < list.len(); ++i) {
		if(list.isFloat(i))
			m.addFloatArg(list.getFloat(i));
		else
			m.addStringArg(list.getSymbol(i));
	}
	Global::instance().sendOscFromAudio(m);
}

//--------------------------------------------------------------
void AudioEngine::receiveNoteOn(const int channel, const int pitch, const int velocity) {
	cout << "OF MIDI: note on: " << channel << " " << pitch << " " << velocity << endl;
}

void AudioEngine::receiveControlChange(const int channel, const int controller, const int value) {
	cout << "OF MIDI: control change: " << channel << " " << controller << " " << value << endl;
}

// note: pgm nums are 1-128 to match pd
void AudioEngine::receiveProgramChange(const int channel, const int value) {
	cout << "OF MIDI: program change: " << channel << " " << value << endl;
}

void AudioEngine::receivePitchBend(const int channel, const int value) {
	cout << "OF MIDI: pitch bend: " << channel << " " << value << endl;
}

void AudioEngine::receiveAftertouch(const int channel, const int value) {
	cout << "OF MIDI: aftertouch: " << channel << " " << value << endl;
}

void AudioEngine::receivePolyAftertouch(const int channel, const int pitch, const int value) {
	cout << "OF MIDI: poly aftertouch: " << channel << " " << pitch << " " << value << endl;
}

// note: pd adds +2 to the port num, so sending to port 3 in pd to [midiout],
//       shows up at port 1 in ofxPd
void AudioEngine::receiveMidiByte(const int port, const int byte) {
	cout << "OF MIDI: midi byte: " << port << " " << byte << endl;
}
