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
#include "Bindings.h"

#include "AppCore.h"
#include "App.h"

float Bindings::appGetMouseX() {return Global::instance().core->parent.mouseX;}
float Bindings::appGetMouseY() {return Global::instance().core->parent.mouseY;}
int Bindings::appGetMouseButton() {return Global::instance().core->mouseButton;}
bool Bindings::appGetMousePressed() {return Global::instance().core->bMousePressed;}

int Bindings::appGetKey() {return Global::instance().core->currentKey;}
bool Bindings::appGetKeyPressed() {return Global::instance().core->bKeyPressed;}
