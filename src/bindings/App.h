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
#pragma once

#include "ofMain.h"
#include "ofxLua.h"

#include "TouchEvent.h"

namespace lua {

class App {

	private:
	
		/// dummy classes for empty class enums
		struct MouseVars {};
		struct KeyboardVars {};

	public:
	
		static void bind(ofxLua& lua) {
		
			using namespace luabind;
			
			module(lua, "app")
			[
				///////////////////////////////
				/// \section App
				
				/// mouse
				def("mouseX", &getMouseX),
				def("mouseY", &getMouseY),
				def("pmouseX", &ofGetPreviousMouseX),
				def("pmouseY", &ofGetPreviousMouseY),
				def("mouseButton", &getMouseButton),
				def("mousePressed", &getMousePressed),
				class_<MouseVars>("mouse")
					.enum_("button")
					[
						value("LEFT", 0),
						value("MIDDLE", 1),
						value("RIGHT", 2)
					],
					
				/// keyboard
				def("keyCode", &getKey),
				def("keyPressed", &getKeyPressed),
				class_<KeyboardVars>("key")
					.enum_("sym")
					[
						value("MODIFIER", OF_KEY_MODIFIER),
						value("RETURN", OF_KEY_RETURN),
						value("ESC", OF_KEY_ESC),
						value("CTRL", OF_KEY_CTRL),
						value("ALT", OF_KEY_ALT),
						value("SHIFT", OF_KEY_SHIFT),
						value("BACKSPACE", OF_KEY_BACKSPACE),
						value("DEL", OF_KEY_DEL),
					
						value("F1", OF_KEY_F1),
						value("F2", OF_KEY_F2),
						value("F3", OF_KEY_F3),
						value("F4", OF_KEY_F4),
						value("F5", OF_KEY_F5),
						value("F6", OF_KEY_F6),
						value("F7", OF_KEY_F7),
						value("F8", OF_KEY_F8),
						value("F9", OF_KEY_F9),
						value("F10", OF_KEY_F10),
						value("F11", OF_KEY_F11),
						value("F12", OF_KEY_F12),
					
						value("LEFT", OF_KEY_LEFT),
						value("RIGHT", OF_KEY_RIGHT),
						value("UP", OF_KEY_UP),
						value("DOWN", OF_KEY_DOWN),
						
						value("PAGE_UP", OF_KEY_PAGE_UP),
						value("PAGE_DOWN", OF_KEY_PAGE_DOWN),
						value("HOME", OF_KEY_HOME),
						value("END", OF_KEY_END),
						value("INSERT", OF_KEY_INSERT)
					],
				
				/// exit
				//def("exit", (void(*)(void)) &ofExit), // shouldn't be able to
				//def("exit", (void(*)(int)) &ofExit), // exit from lua
				
				/// time
				def("frame", &ofGetFrameNum),
				def("frameRate", &ofGetFrameRate),
				def("setFrameRate", &ofSetFrameRate),
				//def("sleepMillis", &ofSleepMillis), // needed? not to be set from lua ...
				def("frameTime", &ofGetLastFrameTime), // seconds
				def("seconds", &ofGetElapsedTimef),
				def("millis", &ofGetElapsedTimeMillis),
				def("micros", &ofGetElapsedTimeMicros),
				def("resetElapsedTime", &ofResetElapsedTimeCounter),
				
				/// cursor
				def("hideCursor", &ofHideCursor),
				def("showCursor", &ofShowCursor),
				
				/// window / screen
				def("width", &getWidth),//&ofGetWidth),
				def("height", &getHeight),//&ofGetHeight),
				// TODO: are these needed by scenes? I'm thinking not
				//def("windowWidth", &ofGetWindowWidth),
				//def("windowHeight", &ofGetWindowHeight),
				//def("windowX", &ofGetWindowPositionX),
				//def("windowY", &ofGetWindowPositionY),
				//def("screenWidth", &ofGetScreenWidth),
				//def("screenHeight", &ofGetScreenHeight),
				//def("windowMode", &ofGetWindowMode), // needed? TODO: needs enums
				
				//def("setWindowTitle", &ofSetWindowTitle)
				// TODO: are these needed by scenes? I'm thinking not
				//def("setWindowPosition", &ofSetWindowPosition),
				//def("setWindowSize", &ofSetWindowShape),
				//def("enableSetupScreen", &ofEnableSetupScreen),
				//def("disableSetupScreen", &ofDisableSetupScreen),
				//def("setFullscreen", &ofSetFullscreen), // needed?
				//def("toggleFullscreen", &ofToggleFullscreen), // needed?
				
				/// sync
				//def("setVerticalSync", &ofSetVerticalSync) // on by default
				
				// path to current scene
				def("scenePath", &getScenePath),
				
				///////////////////////////////
				/// \section Touch Event
				
				class_<TouchEvent>("Touch")
					.def(constructor<>())
					.def_readonly("id", &TouchEvent::id)
					.def_readonly("time", &TouchEvent::time)
					.def_readonly("x", &TouchEvent::x)
					.def_readonly("y", &TouchEvent::y)
					.def_readonly("numTouches", &TouchEvent::numTouches)
					.def_readonly("width", &TouchEvent::width)
					.def_readonly("height", &TouchEvent::height)
					.def_readonly("angle", &TouchEvent::angle)
					.def_readonly("minorAxis", &TouchEvent::minoraxis)
					.def_readonly("majorAxis", &TouchEvent::majoraxis)
					.def_readonly("pressure", &TouchEvent::pressure)
					.def_readonly("xspeed", &TouchEvent::xspeed)
					.def_readonly("yspeed", &TouchEvent::yspeed)
					.def_readonly("xaccel", &TouchEvent::xaccel)
					.def_readonly("yaccel", &TouchEvent::yaccel)
					.enum_("touchType")
					[
						value("DOWN", ofTouchEventArgs::down),
						value("UP", ofTouchEventArgs::up),
						value("MOVE", ofTouchEventArgs::move),
						value("DOUBLETAP", ofTouchEventArgs::doubleTap),
						value("CANCEL", ofTouchEventArgs::cancel)
					]
			];
		}
		
		/// \section Function & Object Wrappers

		static float getWidth();
		static float getHeight();

		/// global mouse vars
		static float getMouseX();
		static float getMouseY();
		static int getMouseButton();
		static bool getMousePressed();
		
		/// global key vars
		static int getKey();
		static bool getKeyPressed();
		
		/// current scene path
		static string getScenePath();
};

} // namespace
