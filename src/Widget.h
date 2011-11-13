/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxPd for documentation
 *
 */
#pragma once

#include "ofMain.h"

/// a desktop os app wrapper
class Widget {

	public:
	
		int screenwidth, screenheight;
		int WRAPWIDTH;
		
		ofRectangle dRect;
		
		float val;
		int init;
		string sendname;
		string receivename;
		string label;
		ofVec2f labelpos;

		ofTrueTypeFont font;
		int fontsize;
		
		Widget() {
		
		}

	private:
	
		static string TAG = "Widget";
};
