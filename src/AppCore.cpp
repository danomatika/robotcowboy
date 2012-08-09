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

AppCore::AppCore(App& parent) : parent(parent),
	global(Global::instance()), sceneManager(parent) {

	mouseButton = 1;
	bMousePressed = false;

	Global::instance().core = this;
}

//--------------------------------------------------------------
void AppCore::setup(const int numOutChannels, const int numInChannels,
				    const int sampleRate, const int ticksPerBuffer) {

	// setup of
	ofSetVerticalSync(true);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(100, 100, 100);
	
	// setup global objects
	global.setup(numOutChannels, numInChannels,
				 sampleRate, ticksPerBuffer);
	
	// load scenes
	sceneManager.add(new Scene(parent, "Test2"));
	sceneManager.setup(false);	// setup all the scenes on the fly
	ofSetLogLevel("ofxSceneManager", OF_LOG_VERBOSE); // lets see whats going on inside
	sceneManager.gotoScene(0, true);
	//parent.setSceneManager(&sceneManager);
	Global::instance().gui.currentScene->setLabel(sceneManager.getCurrentSceneName());
}

//--------------------------------------------------------------
void AppCore::update() {
	
	sceneManager.update();
	
	// check for waiting osc messages
	global.osc.update();
}

//--------------------------------------------------------------
void AppCore::draw() {
	sceneManager.draw();
	
	if(global.scriptEngine.errorOcurred) {
		ofxBitmapString(10, 10) << global.scriptEngine.errorMsg;
	}
}

//--------------------------------------------------------------
void AppCore::exit() {
	sceneManager.clear();
	global.clear();
}

//--------------------------------------------------------------
void AppCore::keyPressed(int key) {

	switch(key) {
	
		case 'r':
			global.scriptEngine.reloadScript();
			break;
			
		default:
			break;
	}
	
	sceneManager.keyPressed(key);
}

//--------------------------------------------------------------
void AppCore::mousePressed(int x, int y, int button) {
	mouseButton = button;
	bMousePressed = true;
	sceneManager.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void AppCore::mouseReleased(int x, int y, int button) {
	bMousePressed = false;
	sceneManager.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void AppCore::audioReceived(float * input, int bufferSize, int nChannels) {
	global.audioEngine.pd.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void AppCore::audioRequested(float * output, int bufferSize, int nChannels) {
	global.audioEngine.pd.audioOut(output, bufferSize, nChannels);
}
