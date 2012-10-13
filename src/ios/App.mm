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
#include "App.h"

#include "../Global.h"
#include "WebServer.h"

//--------------------------------------------------------------
App::App() {}

//--------------------------------------------------------------
void App::setup() {

	// setup iOs
	ofRegisterTouchEvents(this);
//	ofxAccelerometer.setup();
//	ofxiPhoneAlerts.addListener(this);
	
	// if you want a landscape orientation 
	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	
	// setup OF sound stream
	int ticksPerBuffer = 8;	// 8 * 64 = buffer len of 512
	ofSoundStreamSetup(2, 2, this, 44100, ofxPd::blockSize()*ticksPerBuffer, 4);
	
	// setup the app core
	core.setup(2, 2, 44100, ticksPerBuffer);
	
	//WebServer::start(Global::instance().docsPath);
}

//--------------------------------------------------------------
void App::update() {
	core.update();
}

//--------------------------------------------------------------
void App::draw() {
	core.draw();
}

//--------------------------------------------------------------
void App::exit() {
	core.exit();
	
	//WebServer::stop();
}

//--------------------------------------------------------------
void App::touchDown(ofTouchEventArgs &touch) {
	Global::instance().scriptEngine.touchDown(touch);
}

//--------------------------------------------------------------
void App::touchMoved(ofTouchEventArgs &touch) {
	Global::instance().scriptEngine.touchMoved(touch);
}

//--------------------------------------------------------------
void App::touchUp(ofTouchEventArgs &touch) {
	Global::instance().scriptEngine.touchUp(touch);
}

//--------------------------------------------------------------
void App::touchDoubleTap(ofTouchEventArgs &touch) {
	Global::instance().scriptEngine.touchDoubleTap(touch);
}

//--------------------------------------------------------------
void App::touchCancelled(ofTouchEventArgs& touch) {
	Global::instance().scriptEngine.touchCancelled(touch);
}

//--------------------------------------------------------------
void App::lostFocus() {

}

//--------------------------------------------------------------
void App::gotFocus() {

}

//--------------------------------------------------------------
void App::gotMemoryWarning() {

}

//--------------------------------------------------------------
void App::deviceOrientationChanged(int newOrientation) {

}

//--------------------------------------------------------------
void App::audioReceived(float* input, int bufferSize, int nChannels) {
	core.audioReceived(input, bufferSize, nChannels);
}

void App::audioRequested(float* output, int bufferSize, int nChannels) {
	core.audioRequested(output, bufferSize, nChannels);
}
