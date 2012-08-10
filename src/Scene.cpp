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
#include "Scene.h"

#include "App.h"

//--------------------------------------------------------------
Scene::Scene(ofxApp &app, string name) :
	ofxScene(app, name, false), core(dynamic_cast<App&>(app).core),
	global(Global::instance()) {
	path = "";
}

//--------------------------------------------------------------
void Scene::setup()	{

	global.gui.setSceneName(getName());
	path = ofFilePath::getAbsolutePath(core.global.scenePath+getName()+"/");
	
	if(!ofDirectory::doesDirectoryExist(ofFilePath::getEnclosingDirectory(path))) {
		ofLogError() << "Scene: scene path \"" << path << "\" does not exist";
		return;
	}
	
	ofLogVerbose() << "Scene: loading scene: \"" << getName() << "\"";
	
	if(ofFile::doesFileExist(path+"_main.pd")) {
		global.audioEngine.loadPatch(path+"_main.pd");
	}
	
	if(ofFile::doesFileExist(path+"_main.lua")) {
		global.scriptEngine.loadScript(path+"_main.lua");
	}
	
	ofSetDataPathRoot(path);

	global.scriptEngine.lua.scriptSetup();
}

//--------------------------------------------------------------
void Scene::update() {global.scriptEngine.lua.scriptUpdate();}
void Scene::draw() {global.scriptEngine.lua.scriptDraw();}

//--------------------------------------------------------------
void Scene::exit() {
	global.gui.setPlayButtonValue(false);
	global.scriptEngine.clearScript();
	global.audioEngine.clearPatch();
	path = "";
}

//--------------------------------------------------------------		
void Scene::keyPressed(int key) {global.scriptEngine.lua.scriptKeyPressed(key);}
void Scene::mouseMoved(int x, int y) {global.scriptEngine.lua.scriptMouseMoved(x, y);}
void Scene::mouseDragged(int x, int y, int button) {global.scriptEngine.lua.scriptMouseDragged(x, y, button);}
void Scene::mousePressed(int x, int y, int button) {global.scriptEngine.lua.scriptMousePressed(x, y, button);}
void Scene::mouseReleased(int x, int y, int button) {global.scriptEngine.lua.scriptMouseReleased(x, y, button);}
		