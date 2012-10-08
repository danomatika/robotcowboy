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

#include "ofxPd.h"

//--------------------------------------------------------------
App::App() {}

//--------------------------------------------------------------
void App::setup() {

//	#ifdef TARGET_OF_IPHONE
//		ofFile configFile(ofFilePath::getUserHomeDir()+"/.robotcowboy");
//	#else
//		ofFile configFile(ofFilePath::getUserHomeDir()+"/.robotcowboy");
//	#endif
//	
//	// check if location is a dir
//	if(configFile.isDirectory()) {
//		ofLogWarning() << "App: config file location is a directory, cannot load file";
//	}
//	// create file if it dosen't exist
//	else if(!configFile.exists()) {
//		ofLogNotice() << "App: config file dosen't exist, copying default ...";
//		if(!configFile.create()) {
//			ofLogError() << "App: couldn't copy config file!";
//		}
//	}

	// setup OF sound stream
	int ticksPerBuffer = 8;	// 8 * 64 = buffer len of 512
	ofSoundStreamSetup(2, 2, this, 44100, ofxPd::blockSize()*ticksPerBuffer, 4);

	// setup the app core
	core.setup(2, 2, 44100, ticksPerBuffer);
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
}

//--------------------------------------------------------------
void App::keyPressed(int key) {
	core.keyPressed(key);
}

//--------------------------------------------------------------
void App::keyReleased(int key) {
	core.keyReleased(key);
}

//--------------------------------------------------------------
void App::mouseMoved(int x, int y) {
	core.mouseMoved(x, y);
}

//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button) {
	core.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button) {
	core.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button) {
	core.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void App::windowResized(int w, int h) {}

//--------------------------------------------------------------
void App::audioReceived(float * input, int bufferSize, int nChannels) {
	core.audioReceived(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void App::audioRequested(float * output, int bufferSize, int nChannels) {
	core.audioRequested(output, bufferSize, nChannels);
}
