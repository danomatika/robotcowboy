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
#include "LuaWrapper.h"

#include "AppCore.h"
#include "App.h"

float LuaWrapper::getMouseX() {return Global::instance().core->parent.mouseX;}
float LuaWrapper::getMouseY() {return Global::instance().core->parent.mouseY;}
float LuaWrapper::getMouseButton() {return Global::instance().core->mouseButton;}
float LuaWrapper::getMousePressed() {return Global::instance().core->bMousePressed;}
