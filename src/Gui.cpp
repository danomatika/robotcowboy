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
#include "Gui.h"

#include "Global.h"
#include "AppCore.h"

//--------------------------------------------------------------
Gui::Gui() {}

//--------------------------------------------------------------
bool Gui::setup() {

	gui = new ofxUICanvas(0, 0, ofGetWidth(), ofGetHeight());
	
	currentScene = new ofxUILabel("kaa", OFX_UI_FONT_LARGE);
	prevScene = new ofxUIButton(50, 50, false, "Prev");
	nextScene = new ofxUIButton(50, 50, false, "Next");
	play = new ofxUIToggle(50, 50, false, "Play");
	
	gui->addWidgetDown(currentScene);
	gui->addWidgetDown(prevScene);
	gui->addWidgetRight(nextScene);
	gui->addWidgetDown(play);
	
	ofAddListener(gui->newGUIEvent, this, &Gui::guiEvent);
	
	return true;
}

//--------------------------------------------------------------
void Gui::clear() {
	delete gui;
}

//--------------------------------------------------------------
void Gui::guiEvent(ofxUIEventArgs &e) {
		
	if(e.widget == prevScene) {
		if(prevScene->getValue()) {
			Global::instance().core->sceneManager.prevScene();
			currentScene->setLabel(Global::instance().core->sceneManager.getCurrentSceneName());
		}
	}
	else if(e.widget == nextScene) {
		if(nextScene->getValue()) {
			Global::instance().core->sceneManager.nextScene();
			currentScene->setLabel(Global::instance().core->sceneManager.getCurrentSceneName());
		}
	}
	else if(e.widget == play) {
		Global::instance().core->sceneManager.runToggle();
	}
}
