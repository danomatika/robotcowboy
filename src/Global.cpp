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
#include "Global.h"

#include "AppCore.h"

//--------------------------------------------------------------
Global& Global::instance() {
    static Global * pointerToTheSingletonInstance = new Global;
    return *pointerToTheSingletonInstance;
}

void Global::setApp(AppCore* app) {
	this->app = app;
}

// PRIVATE

//--------------------------------------------------------------
Global::Global() : app(NULL),
	oscSendAddress("127.0.0.1"), oscSendPort(8880), oscReceivePort(9009),
	visualSendsOut(false), audioSendsOut(false)
	{}
