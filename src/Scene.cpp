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

string Scene::sceneFolder = "scenes";

//--------------------------------------------------------------
Scene::Scene(ofxApp &app) :
	ofxScene(app, "scene"), core(dynamic_cast<App&>(app).core),
	scriptEngine(core.global.scriptEngine) {
	path = "";
}

//--------------------------------------------------------------
Scene::Scene(ofxApp &app, string path) :
	ofxScene(app, "scene"), core(dynamic_cast<App&>(app).core),
	scriptEngine(core.global.scriptEngine) {
	load(path);
}

//--------------------------------------------------------------
bool Scene::load(string path) {
	cout << "Scene: loading scene at: \"" << sceneFolder+"/"+path << "\"" << endl;
	patch = core.global.audioEngine.pd.openPatch(sceneFolder+"/"+path+"/_main.pd");
	bool ret = patch.isValid();
	ret = ret && scriptEngine.loadScript(sceneFolder+"/"+path+"/_main.lua");
	this->path = path;
	setName(path);
	return ret;
}

//--------------------------------------------------------------
void Scene::exit()	{
	core.global.scriptEngine.lua.scriptExit();
	core.global.scriptEngine.lua.clear();
	core.global.audioEngine.pd.closePatch(patch);
	path = "";
}

//--------------------------------------------------------------
bool Scene::setSceneFolder(string path) {
	if(!ofDirectory::doesDirectoryExist(ofFilePath::getEnclosingDirectory(path))) {
		ofLogError() << "Scene: scene folder path \"" << path << "\" does not exist";
		return false;
	}
	sceneFolder = path;
	return true;
}
		