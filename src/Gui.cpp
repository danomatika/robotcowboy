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
	
	//console.setup(10, 34);
	console.pos.x = 750;
	console.pos.y = 34;
	
	ofAddListener(gui->newGUIEvent, this, &Gui::guiEvent);
	
	return true;
}

//--------------------------------------------------------------
void Gui::clear() {
	delete gui;
}

//--------------------------------------------------------------
void Gui::setSceneName(string name) {
	currentScene->setLabel(name);
}

//--------------------------------------------------------------
void Gui::setPlayButtonValue(bool value) {
	play->setValue(value);
}

//--------------------------------------------------------------
void Gui::guiEvent(ofxUIEventArgs &e) {
		
	ofxSceneManager& sceneManager = Global::instance().core->sceneManager;
		
	if(e.widget == prevScene) {
		if(prevScene->getValue()) {
			Global::instance().core->sceneManager.prevScene();
		}
	}
	else if(e.widget == nextScene) {
		if(nextScene->getValue()) {
			sceneManager.nextScene();
		}
	}
	else if(e.widget == play) {
		sceneManager.run(play->getValue());
	}
}

// CONSOLE

//--------------------------------------------------------------
Gui::Console::Console() {
	width = 40;
	height = 20;
	//font = OFX_UI_FONT_SMALL);
}

//--------------------------------------------------------------
void Gui::Console::setup(int w, int h) {
	clear();
	width = w;
	height = h;
}

//--------------------------------------------------------------
void Gui::Console::addLine(string line) {
	mutex.lock();
	lines.push_back(line);
	while(lines.size() > height)
		lines.pop_front();
	mutex.unlock();
}

//--------------------------------------------------------------
void Gui::Console::clear() {
	mutex.lock();
	lines.clear();
	mutex.unlock();
}

//--------------------------------------------------------------
void Gui::Console::draw() {
	mutex.lock();
	int x = pos.x, y = pos.y;
	deque<string>::iterator iter = lines.begin();
	for(; iter != lines.end(); ++iter) {
		ofDrawBitmapString((*iter), x, y);
		y += 14;
	}
	mutex.unlock();
}
