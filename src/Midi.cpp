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
#include "Midi.h"

#include "Global.h"

//--------------------------------------------------------------
Midi::Midi() {}

//--------------------------------------------------------------
bool Midi::setup() {

	ofxMidi::enableNetworking();
	
	ofxMidiIn::listPorts();
	ofxMidiOut::listPorts();
	
	connect();
	
	ofxMidi::setConnectionListener(this);
	
	return true;
}

void Midi::clear() {
	disconnect();
}

//--------------------------------------------------------------
void Midi::connect() {
	
	// connect to everything if the whitelist is empty
	if(inputNames.empty()) {
		for(int i = 0; i < ofxMidiIn::getNumPorts(); ++i) {
			ofxMidiIn *in = new ofxMidiIn;
			in->addListener(this);
			in->openPort(i);
			inputs.push_back(in);
		}
	}
	// connect to white list
	else {
		for(int i = 0; i < ofxMidiIn::getNumPorts(); ++i) {
			// look in whitelist
			for(int j = 0; j < inputNames.size(); ++j) {
				if(inputNames[j] == ofxMidiIn::getPortName(i)) {
					ofxMidiIn *in = new ofxMidiIn;
					in->addListener(this);
					in->openPort(i);
					inputs.push_back(in);
				}
			}
		}
	}
	
	// outputs
	if(outputNames.empty()) {
		for(int i = 0; i < ofxMidiOut::getNumPorts(); ++i) {
			ofxMidiOut *out = new ofxMidiOut;
			out->openPort(i);
			outputs.push_back(out);
		}
	}
	else {
		for(int i = 0; i < ofxMidiOut::getNumPorts(); ++i) {
			// look in whitelist
			for(int j = 0; j < outputNames.size(); ++j) {
				if(outputNames[j] == ofxMidiOut::getPortName(i)) {
					ofxMidiOut *out = new ofxMidiOut;
					out->openPort(i);
					outputs.push_back(out);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void Midi::disconnect() {
	for(int i = 0; i < inputs.size(); ++i) {
		inputs[i]->closePort();
		delete inputs[i];
	}
	for(int i = 0; i < outputs.size(); ++i) {
		outputs[i]->closePort();
		delete outputs[i];
	}
	inputs.clear();
	outputs.clear();
}

//--------------------------------------------------------------
void Midi::sendNoteOn(const int channel, const int pitch, const int velocity) {
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		(*iter)->sendNoteOn(channel, pitch, velocity);
	}
}

//--------------------------------------------------------------
void Midi::sendControlChange(const int channel, const int controller, const int value) {
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		(*iter)->sendControlChange(channel, controller, value);
	}
}

//--------------------------------------------------------------
void Midi::sendProgramChange(const int channel, const int value) {
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		(*iter)->sendProgramChange(channel, value);
	}
}

//--------------------------------------------------------------
void Midi::sendPitchBend(const int channel, const int value) {
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		(*iter)->sendPitchBend(channel, value);
	}
}

//--------------------------------------------------------------
void Midi::sendAftertouch(const int channel, const int value) {
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		(*iter)->sendAftertouch(channel, value);
	}
}

//--------------------------------------------------------------
void Midi::sendPolyAftertouch(const int channel, const int pitch, const int value) {
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		(*iter)->sendPolyAftertouch(channel, pitch, value);
	}
}

//--------------------------------------------------------------
void Midi::newMidiMessage(ofxMidiMessage& msg) {
	
	AudioEngine& audioEngine = Global::instance().audioEngine;
	
	switch(msg.status) {
	
		case MIDI_NOTE_ON:
			audioEngine.pd.sendNoteOn(msg.channel, msg.pitch, msg.velocity);
			break;
			
		case MIDI_NOTE_OFF:
			audioEngine.pd.sendNoteOn(msg.channel, msg.pitch, 0);
			break;
			
		case MIDI_CONTROL_CHANGE:
			audioEngine.pd.sendControlChange(msg.channel, msg.control, msg.value);
			break;
			
		case MIDI_PROGRAM_CHANGE:
			audioEngine.pd.sendProgramChange(msg.channel, msg.value);
			break;
			
		case MIDI_PITCH_BEND:
			audioEngine.pd.sendPitchBend(msg.channel, msg.value);
			break;
		
		case MIDI_AFTERTOUCH:
			audioEngine.pd.sendAftertouch(msg.channel, msg.value);
			break;
			
		case MIDI_POLY_AFTERTOUCH:
			audioEngine.pd.sendPolyAftertouch(msg.channel, msg.pitch, msg.value);
			break;
			
		default:
			break;
	}
}

//--------------------------------------------------------------
void Midi::midiInputAdded(string name, bool isNetwork) {
	
	stringstream msg;
	msg << "ofxMidi: input added: " << name << " network: " << isNetwork;
	
	// create and open a new input port
	ofxMidiIn * newInput = new ofxMidiIn;
	newInput->openPort(name);
	newInput->addListener(this);
	inputs.push_back(newInput);
}

//--------------------------------------------------------------
void Midi::midiInputRemoved(string name, bool isNetwork) {
	stringstream msg;
	msg << "Midi: input removed: " << name << " network: " << isNetwork << endl;
	
	// close and remove input port
	vector<ofxMidiIn*>::iterator iter;
	for(iter = inputs.begin(); iter != inputs.end(); ++iter) {
		ofxMidiIn * input = (*iter);
		if(input->getName() == name) {
			input->closePort();
			input->removeListener(this);
			delete input;
			inputs.erase(iter);
			break;
		}
	}
}

//--------------------------------------------------------------
void Midi::midiOutputAdded(string name, bool isNetwork) {
	stringstream msg;
	msg << "Midi: output added: " << name << " network: " << isNetwork << endl;
	
	// create and open new output port
	ofxMidiOut * newOutput = new ofxMidiOut;
	newOutput->openPort(name);
	outputs.push_back(newOutput);
}

//--------------------------------------------------------------
void Midi::midiOutputRemoved(string name, bool isNetwork) {
	stringstream msg;
	msg << "Midi: output removed: " << name << " network: " << isNetwork << endl;
	
	// close and remove output port
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		ofxMidiOut * output = (*iter);
		if(output->getName() == name) {
			output->closePort();
			delete output;
			outputs.erase(iter);
			break;
		}
	}
}
