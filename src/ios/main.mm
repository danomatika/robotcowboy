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

//========================================================================
int main() {

	ofSetupOpenGL(1024, 768, OF_FULLSCREEN);

	ofRunApp(new App);
}
