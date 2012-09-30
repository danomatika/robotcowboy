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
#include "AudioEngine.h"

#include "ofxOsc.h"

#include "Global.h"
#include "externals/Externals.h"
#include "poco/StringTokenizer.h"

//--------------------------------------------------------------
AudioEngine::AudioEngine() {
	sendsOscOut = false;
}

//--------------------------------------------------------------
bool AudioEngine::setup(const int numOutChannels, const int numInChannels,
						const int sampleRate, const int ticksPerBuffer) {

	if(!pd.init(numOutChannels, numInChannels, sampleRate, ticksPerBuffer)) {
		ofLogError() << "AudioEngine: Could not init pd";
		return false;
	}

	Externals::setup();
	pd.subscribe("OSC_OUT");
	pd.addToSearchPath("externals");
	pd.addToSearchPath("externals/rj");
	pd.start();
	return true;
}

//--------------------------------------------------------------
void AudioEngine::clearPatch() {
	if(currentPatch.isValid()) {
		pd.closePatch(currentPatch);
	}
}

//--------------------------------------------------------------
bool AudioEngine::loadPatch(string patch) {
	ofLogVerbose() << "AudioEngine: loading patch \"" << ofFilePath::getFileName(patch) << "\"";
	clearPatch();
	currentPatch = pd.openPatch(patch);
	return currentPatch.isValid();
}

//--------------------------------------------------------------
bool AudioEngine::reloadPatch() {
	pd::Patch p = currentPatch;
	clearPatch();
	currentPatch = pd.openPatch(p);
	return currentPatch.isValid();
}

//--------------------------------------------------------------
void AudioEngine::update() {

	while(pd.numMessages() > 0) {
	
		pd::Message& msg = pd.nextMessage();
		switch(msg.type) {
			
			case pd::PRINT:
				ofLogNotice() << "PD: " << msg.symbol;
				Global::instance().gui.console.addLine("PD: " + msg.symbol);
				break;
			
			// events
			case pd::BANG:
			{
				ofxOscMessage m;
				m.setAddress(msg.dest);
				Global::instance().osc.sendOscFromAudio(m);
				break;
			}
			case pd::FLOAT:
				break;
			case pd::SYMBOL:
			{
				ofxOscMessage m;
				m.setAddress(msg.symbol);
				Global::instance().osc.sendOscFromAudio(m);
				break;
			}
			case pd::LIST:
			{
				if(!msg.list.isSymbol(0)) {
					ofLogWarning() << "AudioEngine: Malformed osc message, ignoring" << endl;
					return;
				}
				
				ofxOscMessage m;
				for(int i = 0; i < msg.list.len(); ++i) {
					if(msg.list.isFloat(i))
						m.addFloatArg(msg.list.getFloat(i));
					else
						m.addStringArg(msg.list.getSymbol(i));
				}
				Global::instance().osc.sendOscFromAudio(m);
				break;
			}
			case pd::MESSAGE:
			{
				ofxOscMessage m;
				m.setAddress(msg.symbol);
				for(int i = 0; i < msg.list.len(); ++i) {
					if(msg.list.isFloat(i))
						m.addFloatArg(msg.list.getFloat(i));
					else
						m.addStringArg(msg.list.getSymbol(i));
				}
				Global::instance().osc.sendOscFromAudio(m);
				break;
			}
			
			// midi
			case pd::NOTE_ON:
				Global::instance().midi.sendNoteOn(msg.channel, msg.pitch, msg.velocity);
				break;
			case pd::CONTROL_CHANGE:
				Global::instance().midi.sendControlChange(msg.channel, msg.controller, msg.value);
				break;
			case pd::PROGRAM_CHANGE:
				Global::instance().midi.sendProgramChange(msg.channel, msg.value);
				break;
			case pd::PITCH_BEND:
				Global::instance().midi.sendPitchBend(msg.channel, msg.value);
				break;
			case pd::AFTERTOUCH:
				Global::instance().midi.sendAftertouch(msg.channel, msg.value);
				break;
			case pd::POLY_AFTERTOUCH:
				Global::instance().midi.sendPolyAftertouch(msg.channel, msg.pitch, msg.value);
				break;
			case pd::BYTE:
				break;
		
			case pd::NONE:
				break;
		}
	}
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
				pd.addFloat(msg.getArgAsFloat(i));
				break;
			case OFXOSC_TYPE_STRING:
				pd.addSymbol(msg.getArgAsString(i));
				break;
		}
	}
	pd.finishList("OSC_IN");
}

//--------------------------------------------------------------
void AudioEngine::sendOsc(ofxOscBundle& bundle) {
	
	// recurse into bundles
	for(int i = 0; i < bundle.getBundleCount(); ++i) {
		sendOsc(bundle.getBundleAt(i));
	}
	
	// process messages
	for(int i = 0; i < bundle.getMessageCount(); ++i) {
		sendOsc(bundle.getMessageAt(i));
	}
}
