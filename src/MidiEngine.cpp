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
#include "MidiEngine.h"

#include "Global.h"

//--------------------------------------------------------------
MidiEngine::MidiEngine() {}

//--------------------------------------------------------------
bool MidiEngine::setup() {

	ofxMidi::enableNetworking();
	
	ofxMidiIn::listPorts();
	ofxMidiOut::listPorts();
	
	for(int i = 0; i < ofxMidiIn::getNumPorts(); ++i) {
		inputs.push_back(new ofxMidiIn);
		inputs[i]->addListener(this);
		inputs[i]->openPort(i);
	}
	
	for(int i = 0; i < ofxMidiOut::getNumPorts(); ++i) {
		outputs.push_back(new ofxMidiOut);
		outputs[i]->openPort(i);
	}
	
	ofxMidi::setConnectionListener(this);
	
	return true;
}

//--------------------------------------------------------------
void MidiEngine::sendNoteOn(const int channel, const int pitch, const int velocity) {
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		(*iter)->sendNoteOn(channel, pitch, velocity);
	}
}

//--------------------------------------------------------------
void MidiEngine::sendControlChange(const int channel, const int controller, const int value) {
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		(*iter)->sendControlChange(channel, controller, value);
	}
}

//--------------------------------------------------------------
void MidiEngine::sendProgramChange(const int channel, const int value) {
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		(*iter)->sendProgramChange(channel, value);
	}
}

//--------------------------------------------------------------
void MidiEngine::sendPitchBend(const int channel, const int value) {
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		(*iter)->sendPitchBend(channel, value);
	}
}

//--------------------------------------------------------------
void MidiEngine::sendAftertouch(const int channel, const int value) {
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		(*iter)->sendAftertouch(channel, value);
	}
}

//--------------------------------------------------------------
void MidiEngine::sendPolyAftertouch(const int channel, const int pitch, const int value) {
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		(*iter)->sendPolyAftertouch(channel, pitch, value);
	}
}

//--------------------------------------------------------------
void MidiEngine::newMidiMessage(ofxMidiMessage& msg) {
	
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
	}
}

//--------------------------------------------------------------
void MidiEngine::midiInputAdded(string name, bool isNetwork) {
	
	stringstream msg;
	msg << "ofxMidi: input added: " << name << " network: " << isNetwork;
	
	// create and open a new input port
	ofxMidiIn * newInput = new ofxMidiIn;
	newInput->openPort(name);
	newInput->addListener(this);
	inputs.push_back(newInput);
}

//--------------------------------------------------------------
void MidiEngine::midiInputRemoved(string name, bool isNetwork) {
	stringstream msg;
	msg << "MidiEngine: input removed: " << name << " network: " << isNetwork << endl;
	
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
void MidiEngine::midiOutputAdded(string name, bool isNetwork) {
	stringstream msg;
	msg << "MidiEngine: output added: " << name << " network: " << isNetwork << endl;
	
	// create and open new output port
	ofxMidiOut * newOutput = new ofxMidiOut;
	newOutput->openPort(name);
	outputs.push_back(newOutput);
}

//--------------------------------------------------------------
void MidiEngine::midiOutputRemoved(string name, bool isNetwork) {
	stringstream msg;
	msg << "MidiEngine: output removed: " << name << " network: " << isNetwork << endl;
	
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
