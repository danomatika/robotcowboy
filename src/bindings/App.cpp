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

//#include "../Global.h"
#include "../AppCore.h"
#include "../desktop/App.h"

namespace lua {

float App::getWidth() {return Global::instance().core->parent.getRenderWidth();}
float App::getHeight() {return Global::instance().core->parent.getRenderHeight();}

float App::getMouseX() {
//	if(ofGetWindowMode() == OF_FULLSCREEN)
//		return Global::instance().core->parent.mouseX + Global::instance().core->parent.getOriginX();
//	else
		return Global::instance().core->parent.mouseX;
}
float App::getMouseY() {
//	if(ofGetWindowMode() == OF_FULLSCREEN)
//		return Global::instance().core->parent.mouseY + Global::instance().core->parent.getOriginY();
//	else
		return Global::instance().core->parent.mouseY;
}
int App::getMouseButton() {return Global::instance().core->mouseButton;}
bool App::getMousePressed() {return Global::instance().core->bMousePressed;}

int App::getKey() {return Global::instance().core->currentKey;}
bool App::getKeyPressed() {return Global::instance().core->bKeyPressed;}

} // namespace
