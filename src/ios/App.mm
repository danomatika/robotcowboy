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

#include "Global.h"

//--------------------------------------------------------------
App::App() {}

//--------------------------------------------------------------
void App::setup() {

	// setup iOs
//	ofRegisterTouchEvents(this);
//	ofxAccelerometer.setup();
//	ofxiPhoneAlerts.addListener(this);
	//ofSetDataPathRoot("./data/");
	
	// if you want a landscape orientation 
	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT;
	
	cout << "cwd: " << ofFilePath::getCurrentWorkingDirectory() << endl;
	
	ofBackground(127, 127, 127);
	
	// the number if libpd ticks per buffer,
	// used to compute the audio buffer len: tpb * blocksize (always 64)
	int ticksPerBuffer = 8;	// 8 * 64 = buffer len of 512
	
	// setup OF sound stream
	ofSoundStreamSetup(2, 2, this, 44100, ofxPd::blockSize()*ticksPerBuffer, 4);
	
	// setup the app core
	core.setup(2, 2, 44100, ticksPerBuffer);
	
	bangSound.loadSound("data/samples/bang.wav");
	bangSound.setLoop(false);
	bangSound.setVolume(0.75);
	bangSound.play();
}

//--------------------------------------------------------------
void App::update() {
	core.update();
	ofSoundUpdate();
}

//--------------------------------------------------------------
void App::draw() {
	core.draw();
	if(core.sceneManager.getCurrentSceneIndex() > -1) {
		ofDrawBitmapString(core.sceneManager.getCurrentSceneName(), 100, 100);
	}
}

//--------------------------------------------------------------
void App::exit() {
	core.exit();
}

//--------------------------------------------------------------
void App::touchDown(ofTouchEventArgs &touch) {
	cout << "TouchDown: " << touch.x << " " << touch.y << endl;
	bangSound.play();
	Global::instance().audioEngine.pd.sendSymbol("OSC_IN", "/transport");
}

//--------------------------------------------------------------
void App::touchMoved(ofTouchEventArgs &touch) {

}

//--------------------------------------------------------------
void App::touchUp(ofTouchEventArgs &touch) {

}

//--------------------------------------------------------------
void App::touchDoubleTap(ofTouchEventArgs &touch) {
	cout << "TouchDoubleTap: " << touch.x << " " << touch.y << endl;
	core.sceneManager.nextScene();
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
void App::touchCancelled(ofTouchEventArgs& args) {

}

//--------------------------------------------------------------
void App::audioReceived(float* input, int bufferSize, int nChannels) {
	core.audioReceived(input, bufferSize, nChannels);
}

void App::audioRequested(float* output, int bufferSize, int nChannels) {
	core.audioRequested(output, bufferSize, nChannels);
}
