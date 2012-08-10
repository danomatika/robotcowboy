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

class Gui {

	public:
	
		Gui();
	
		bool setup();
		void clear();
		
		void setSceneName(string name);
		void setPlayButtonValue(bool value);
		
		// gui callback
		void guiEvent(ofxUIEventArgs &e);
		
		ofxUICanvas *gui;
		
		ofxUILabel *currentScene;
		ofxUIButton *prevScene;
		ofxUIButton *nextScene;
		ofxUIButton *play;
};
