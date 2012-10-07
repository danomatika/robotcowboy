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
#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "../AppCore.h"

class App : public ofxiPhoneApp {
	
	public:
	
		App();
	
		void setup();
		void update();
		void draw();
		void exit();
		
		void touchDown(ofTouchEventArgs &touch);
		void touchMoved(ofTouchEventArgs &touch);
		void touchUp(ofTouchEventArgs &touch);
		void touchDoubleTap(ofTouchEventArgs &touch);
		void touchCancelled(ofTouchEventArgs &touch);

		void lostFocus();
		void gotFocus();
		void gotMemoryWarning();
		void deviceOrientationChanged(int newOrientation);
		
		// audio callbacks
		void audioReceived(float* input, int bufferSize, int nChannels);
		void audioRequested(float* output, int bufferSize, int nChannels);
		
		// webdav server
		void startWebServer();
		void stopWebServer();
		bool isWebServerRunning();

		AppCore core;
};


