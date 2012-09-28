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
#pragma once

#include "ofMain.h"

#include "ofxOsc.h"
#include "ofxAppUtils.h"

#include "Global.h"

class App;
//class Global;

class AppCore {

	public:

		AppCore(App& parent);

		// main
		void setup(const int numOutChannels, const int numInChannels,
				   const int sampleRate, const int ticksPerBuffer);
		void update();
		void draw();
        void exit();
		
		// input callbacks
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
		// audio callbacks
		void audioReceived(float * input, int bufferSize, int nChannels);
		void audioRequested(float * output, int bufferSize, int nChannels);
		
		/// for processing heads
		int mouseButton;
		bool bMousePressed;
		int currentKey;
		bool bKeyPressed;
		
		App& parent;
		
		Global& global;
		
		ofxSceneManager sceneManager;
};
