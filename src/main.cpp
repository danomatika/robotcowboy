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
#include "ofMain.h"
#include "App.h"
#include "ofAppGlutWindow.h"

#include "ofxAppUtils.h"

//========================================================================
int main(){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 200, 200, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunAppWithAppUtils(new App());

}
