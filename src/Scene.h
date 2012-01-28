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
#pragma once

#include "ofMain.h"

#include "ofxPd.h"
#include "ofxAppUtils.h"

#include "AppCore.h"

class Scene : public ofxScene {

	public:
	
		Scene(ofxApp &app);
		Scene(ofxApp &app, string path);
		
		/// load the scene
		bool load(string path);
		
		/// is the scene loaded?
		bool isLoaded() {return path == "" ? false : true;}
		
		/// \section Scene callbacks
		
		/// run the script!
		void setup()	{core.scriptEngine.lua.scriptSetup();}
		void update()	{core.scriptEngine.lua.scriptUpdate();}
		void draw()		{core.scriptEngine.lua.scriptDraw();}
		void exit();
		
		void keyPressed(int key)						{core.scriptEngine.lua.scriptKeyPressed(key);}
		void mouseMoved(int x, int y)					{core.scriptEngine.lua.scriptMouseMoved(x, y);}
		void mouseDragged(int x, int y, int button)		{core.scriptEngine.lua.scriptMouseDragged(x, y, button);}
		void mousePressed(int x, int y, int button)		{core.scriptEngine.lua.scriptMousePressed(x, y, button);}
		void mouseReleased(int x, int y, int button)	{core.scriptEngine.lua.scriptMouseReleased(x, y, button);}
		
		/// \section Globals
		
		/// set/get the main scene folder
		static bool setSceneFolder(string path);
		static string getSceneFolder()	{return sceneFolder;}
		
	private:
	
		AppCore& core;
	
		string path;		//< path to this scene
		pd::Patch patch;	//< pd patch handle

		static string sceneFolder;	//< where to look for scenes
};
