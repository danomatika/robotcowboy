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

#include "ofxPd.h"
#include "ofxAppUtils.h"

#include "AppCore.h"

class Scene : public ofxScene {

	public:
	
		/// name should be the same as the scene folder name!
		Scene(ofxApp &app, string name);
		
		void setup();
		void update();
		void draw();
		void exit();
		
		void keyPressed(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
	private:
	
		AppCore& core;
		Global& global;
	
		string path;	//< path to this scene
};
