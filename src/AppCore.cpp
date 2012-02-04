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

#include "App.h"
#include "Scene.h"
#include "Global.h"
#include "poco/StringTokenizer.h"

AppCore::AppCore(App& parent) : parent(parent),
	sender(Global::instance().oscSender),
	receiver(Global::instance().oscReceiver),
	audioEngine(Global::instance().audioEngine),
	scriptEngine(Global::instance().scriptEngine),
	sceneManager(parent) {}

//--------------------------------------------------------------
void AppCore::setup(const int numOutChannels, const int numInChannels,
				    const int sampleRate, const int ticksPerBuffer) {

	// setup of
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(100, 100, 100);
	
	// setup osc
	sender.setup(Global::instance().oscSendAddress,
				 Global::instance().oscSendPort);
	receiver.setup(Global::instance().oscReceivePort);
	
	// setup pd and lua
	Global::instance().audioEngine.setup(numOutChannels, numInChannels,
										 sampleRate, ticksPerBuffer);
	Global::instance().scriptEngine.setup();
	
	// load scenes
	sceneManager.add(new Scene(parent, "GuitarTest"));
	sceneManager.setup(false);	// setup all the scenes on the fly
	ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside
	sceneManager.gotoScene(0, true);
	//parent.setSceneManager(&sceneManager);
}

//--------------------------------------------------------------
void AppCore::update() {
	
	sceneManager.update();
	
	// check for waiting osc messages
	while(receiver.hasWaitingMessages()) {
		
		ofxOscMessage msg;
		receiver.getNextMessage(&msg);
	
		// get the main destination address
		Poco::StringTokenizer tokenizer(msg.getAddress(), "/");
		Poco::StringTokenizer::Iterator iter = tokenizer.begin()+1;
		string dest = (*iter);
		
		// give to lua or pd
		if(dest == "visual") {
			scriptEngine.sendOsc(msg);
		}
		else if(dest == "audio") {
			audioEngine.sendOsc(msg);
		}
		else {
			ofLogWarning() << "Unhandled osc message: " << msg.getAddress();
		}
	}
}

//--------------------------------------------------------------
void AppCore::draw() {
	sceneManager.draw();
}

//--------------------------------------------------------------
void AppCore::exit() {
	sceneManager.clear();
	audioEngine.clear();
	scriptEngine.clear();
}

//--------------------------------------------------------------
void AppCore::keyPressed(int key) {

	switch(key) {
	
		case 'r':
			scriptEngine.reloadScript();
			break;
			
		default:
			break;
	}
	
	sceneManager.keyPressed(key);
}

//--------------------------------------------------------------
void AppCore::mousePressed(int x, int y, int button) {
	sceneManager.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void AppCore::audioReceived(float * input, int bufferSize, int nChannels) {
	audioEngine.pd.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void AppCore::audioRequested(float * output, int bufferSize, int nChannels) {
	audioEngine.pd.audioOut(output, bufferSize, nChannels);
}
