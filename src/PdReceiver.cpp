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
#include "PdReceiver.h"

#include "ofxOsc.h"

#include "AppCore.h"
#include "Global.h"

PdReceiver::PdReceiver(AppCore& app) : app(app),
	sender(Global::instance().getOscSender()) {}

void PdReceiver::print(const std::string& message) {
	cout << "PD: " << message << endl;
}

void PdReceiver::receiveBang(const std::string& dest) {
	cout << "OF: bang " << dest << endl;
	
	ofxOscMessage m;
	m.setAddress(dest);
	if(Global::instance().audioSendsOut) {
		sender.sendMessage(m);
	}
	else {
		app.scriptOscReceived(m);
	}
}

void PdReceiver::receiveFloat(const std::string& dest, float value) {
	cout << "OF: float " << dest << ": " << value << endl;
	
	ofxOscMessage m;
	m.setAddress(dest);
	m.addFloatArg(value);
	if(Global::instance().audioSendsOut) {
		sender.sendMessage(m);
	}
	else {
		app.scriptOscReceived(m);
	}
}

void PdReceiver::receiveSymbol(const std::string& dest, const std::string& symbol) {
	cout << "OF: symbol " << dest << ": " << symbol << endl;
	
	ofxOscMessage m;
	m.setAddress(dest);
	m.addStringArg(symbol);
	if(Global::instance().audioSendsOut) {
		sender.sendMessage(m);
	}
	else {
		app.scriptOscReceived(m);
	}
}

void PdReceiver::receiveList(const std::string& dest, const pd::List& list) {
	cout << "OF: list " << dest << ": " << list.toString() << endl;
	
	ofxOscMessage m;
	m.setAddress(dest);
	for(int i = 0; i < list.len(); ++i) {
		if(list.isFloat(i))
			m.addFloatArg(list.asFloat(i));
		else
			m.addStringArg(list.asSymbol(i));
	}
	
	if(Global::instance().audioSendsOut) {
		sender.sendMessage(m);
	}
	else {
		app.scriptOscReceived(m);
	}
}

void PdReceiver::receiveMessage(const std::string& dest, const std::string& msg, const pd::List& list) {
	cout << "OF: message " << dest << ": " << msg << " " << list.toString() << list.types() << endl;
	
	ofxOscMessage m;
	m.setAddress(dest);
	m.addStringArg(msg);
	for(int i = 0; i < list.len(); ++i) {
		if(list.isFloat(i))
			m.addFloatArg(list.asFloat(i));
		else
			m.addStringArg(list.asSymbol(i));
	}
	
	if(Global::instance().audioSendsOut) {
		sender.sendMessage(m);
	}
	else {
		app.scriptOscReceived(m);
	}
}

//--------------------------------------------------------------
void PdReceiver::receiveNoteOn(const int channel, const int pitch, const int velocity) {
	cout << "OF MIDI: note on: " << channel << " " << pitch << " " << velocity << endl;
}

void PdReceiver::receiveControlChange(const int channel, const int controller, const int value) {
	cout << "OF MIDI: control change: " << channel << " " << controller << " " << value << endl;
}

// note: pgm nums are 1-128 to match pd
void PdReceiver::receiveProgramChange(const int channel, const int value) {
	cout << "OF MIDI: program change: " << channel << " " << value << endl;
}

void PdReceiver::receivePitchBend(const int channel, const int value) {
	cout << "OF MIDI: pitch bend: " << channel << " " << value << endl;
}

void PdReceiver::receiveAftertouch(const int channel, const int value) {
	cout << "OF MIDI: aftertouch: " << channel << " " << value << endl;
}

void PdReceiver::receivePolyAftertouch(const int channel, const int pitch, const int value) {
	cout << "OF MIDI: poly aftertouch: " << channel << " " << pitch << " " << value << endl;
}

// note: pd adds +2 to the port num, so sending to port 3 in pd to [midiout],
//       shows up at port 1 in ofxPd
void PdReceiver::receiveMidiByte(const int port, const int byte) {
	cout << "OF MIDI: midi byte: " << port << " " << byte << endl;
}
