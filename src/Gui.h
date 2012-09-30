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

#include "ofxUI.h"

#include <deque>

class Console;

class Gui {

	public:
	
		Gui();
	
		bool setup();
		void clear();
		
		void setSceneName(string name);
		void setPlayButtonValue(bool value);
		
		// gui callback
		void guiEvent(ofxUIEventArgs &e);
		
		void update();
		void draw();
		void exit();
		
		void drawFps();
		
		ofxUICanvas *gui;
		
		ofxUILabel *currentScene;
		ofxUIButton *prevScene;
		ofxUIButton *nextScene;
		ofxUIButton *play;
		
		class Console {

			public:

				Console();

				void setup(int width, int height);

				// add a line to the buffer
				void addLine(string line);

				// clear the buffer
				void clear();

				// draw the console using the current position
				void draw();

				deque<string> lines;
				int width, height; // console chars, not pixels
				ofPoint pos;
				
				ofTrueTypeFont *font;
				
				ofMutex mutex;
				
		} console;
};
