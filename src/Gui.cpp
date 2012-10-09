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
Gui::Gui() {
	gui = NULL;
	currentScene = NULL;
	prevScene = NULL;
	nextScene = NULL;
	play = NULL;
}

//--------------------------------------------------------------
bool Gui::setup() {

	gui = new ofxUICanvas(0, 0, ofGetWidth(), ofGetHeight());
	gui->disableAppEventCallbacks();
	
	currentScene = new ofxUILabel("No Scene", OFX_UI_FONT_LARGE);
	prevScene = new ofxUIButton("Prev", false, 50, 50);
	nextScene = new ofxUIButton("Next", false, 50, 50);
	play = new ofxUIToggle("Play", false, 50, 50);
	
	gui->addWidgetDown(currentScene);
	gui->addWidgetDown(prevScene);
	gui->addWidgetRight(nextScene);
	gui->addWidgetDown(play);
	
	//console.setup(10, 34);
	console.pos.x = 10;
	console.pos.y = ofGetHeight() - (console.height+1)*14;
	ofAddListener(gui->newGUIEvent, this, &Gui::guiEvent);
	
	return true;
}

//--------------------------------------------------------------
void Gui::clear() {
	if(gui)
		gui->exit();
	delete gui;
}

//--------------------------------------------------------------
void Gui::setSceneName(string name) {
	currentScene->setLabel(name);
	ofSetWindowTitle(name);
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

//--------------------------------------------------------------
void Gui::update() {
	if(gui)
		gui->update();
}

//--------------------------------------------------------------
void Gui::draw() {
	if(gui)
		gui->draw();
}

//--------------------------------------------------------------
void Gui::exit() {
	if(gui)
		gui->exit();
}

//--------------------------------------------------------------
void Gui::drawFps() {
	ofxBitmapString(ofGetWidth()-32, 14) << (int) ofGetFrameRate();
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
		ofSetColor(0);
		ofDrawBitmapString((*iter), x+1, y+1);
		ofSetColor(255);
		ofDrawBitmapString((*iter), x, y);
		y += 14;
	}
	mutex.unlock();
}
