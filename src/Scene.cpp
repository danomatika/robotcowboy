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

//--------------------------------------------------------------
Scene::Scene(string name) :
	ofxScene(name, false),
	global(Global::instance()) {
	path = "";
}

//--------------------------------------------------------------
void Scene::setup()	{

	// reset graphics state
	global.resetGraphics();
	
	// set scene name
	global.gui.setSceneName(getName());
	
	// change the current dir to the scene directory, this allows the lua state
	// to find local files
	path = ofFilePath::join(global.scenePath, getName());
	if(!ofDirectory::doesDirectoryExist(ofFilePath::getEnclosingDirectory(path))) {
		ofLogError() << "Scene: scene path \"" << path << "\" does not exist";
		return;
	}
	else {
		if(chdir(path.c_str()) != 0) {
			ofLogError() << "Scene: couldn't change directory to \"" << path << "\"";
			return;
		}
	}
	ofLogVerbose() << "Scene: current dir: " << ofFilePath::getCurrentWorkingDirectory();
	
	// try to load scene
	ofLogVerbose() << "Scene: loading scene: \"" << getName() << "\"";
	if(ofFile::doesFileExist(ofFilePath::join(path, "_main.pd"))) {
		global.audioEngine.loadPatch(ofFilePath::join(path, "_main.pd"));
	}
	
	if(ofFile::doesFileExist(ofFilePath::join(path, "_main.lua"))) {
		global.scriptEngine.loadScript(ofFilePath::join(path, "_main.lua"));
	}

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
void Scene::keyReleased(int key) {global.scriptEngine.lua.scriptKeyReleased(key);}
void Scene::mouseMoved(int x, int y) {global.scriptEngine.lua.scriptMouseMoved(x, y);}
void Scene::mouseDragged(int x, int y, int button) {global.scriptEngine.lua.scriptMouseDragged(x, y, button);}
void Scene::mousePressed(int x, int y, int button) {global.scriptEngine.lua.scriptMousePressed(x, y, button);}
void Scene::mouseReleased(int x, int y, int button) {global.scriptEngine.lua.scriptMouseReleased(x, y, button);}
		