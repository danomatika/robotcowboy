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
#include "ScriptEngine.h"

#include "ofxOsc.h"

#include "Global.h"
#include "bindings/Bindings.h"

//--------------------------------------------------------------
ScriptEngine::ScriptEngine() {
	currentScript = "";
	writeBuffer = "";
	sendsOscOut = false;
	
	lua.addListener(this);
}

//--------------------------------------------------------------
bool ScriptEngine::setup() {
	if(!lua.init(true)) {
		ofLogError() << "ScriptEngine: Could not init lua";
		return false;
	}
	return true;
}

//--------------------------------------------------------------
void ScriptEngine::clear() {
	lua.clear();
	currentScript = "";
	Global::instance().clearError();
}

//--------------------------------------------------------------
void ScriptEngine::clearScript() {
	lua.scriptExit();
	clear();
}

//--------------------------------------------------------------
bool ScriptEngine::loadScript(string script) {
	ofLogVerbose() << "ScriptEngine: loading script \"" << ofFilePath::getFileName(script) << "\"";
	clearScript();
	if(!setup())
		return false;
	lua.bind<Bindings>();
	currentScript = script;
	lua.doScript(bootScript);
	return lua.doScript(script);
}

//--------------------------------------------------------------
bool ScriptEngine::reloadScript() {
	ofLogVerbose() << "ScriptEngine: reloading script \"" << ofFilePath::getFileName(currentScript) << "\"";
	lua.clear();
	Global::instance().clearError();
	if(!setup())
		return false;
	lua.bind<Bindings>();
	lua.doScript(bootScript);
	return lua.doScript(currentScript);
}

//--------------------------------------------------------------
// calling lua functions with objects:
// http://www.nuclex.org/articles/cxx/1-quick-introduction-to-luabind
void ScriptEngine::sendOsc(ofxOscMessage& msg) {
	
	if(!lua.isValid()) {
		return;
	}
	
	try {
		luabind::call_function<bool>(lua, "oscReceived", boost::ref(msg));
	}
	catch(exception& e) {
		//ofLogWarning() << "ScriptEngine::sendOsc: Caught exception: " << e.what();
	}
}

//--------------------------------------------------------------
void ScriptEngine::touchDown(ofTouchEventArgs &touch) {
	
	if(!lua.isValid()) {
		return;
	}
	
	// convert and store
	touchEvent.copyFromArgs(touch);
	
	try {
		luabind::call_function<bool>(lua, "touchDown", boost::ref(touchEvent));
	}
	catch(exception& e) {
		//ofLogWarning() << "ScriptEngine::touchDown: Caught exception: " << e.what();
	}
}

//--------------------------------------------------------------
void ScriptEngine::touchMoved(ofTouchEventArgs &touch) {
	
	if(!lua.isValid()) {
		return;
	}
	
	// convert and store
	touchEvent.copyFromArgs(touch);
	
	try {
		luabind::call_function<bool>(lua, "touchMoved", boost::ref(touchEvent));
	}
	catch(exception& e) {
		//ofLogWarning() << "ScriptEngine::touchMoved: Caught exception: " << e.what();
	}
}

//--------------------------------------------------------------
void ScriptEngine::touchUp(ofTouchEventArgs &touch) {
		
	if(!lua.isValid()) {
		return;
	}
	
	// convert and store
	touchEvent.copyFromArgs(touch);
	
	try {
		luabind::call_function<bool>(lua, "touchUp", boost::ref(touchEvent));
	}
	catch(exception& e) {
		//ofLogWarning() << "ScriptEngine::touchUp: Caught exception: " << e.what();
	}	
}

//--------------------------------------------------------------
void ScriptEngine::touchDoubleTap(ofTouchEventArgs &touch) {
	
	if(!lua.isValid()) {
		return;
	}
	
	// convert and store
	touchEvent.copyFromArgs(touch);
	
	try {
		luabind::call_function<bool>(lua, "touchDoubleTap", boost::ref(touchEvent));
	}
	catch(exception& e) {
		//ofLogWarning() << "ScriptEngine::touchDoubleTap: Caught exception: " << e.what();
	}
}

//--------------------------------------------------------------
void ScriptEngine::touchCancelled(ofTouchEventArgs &touch) {
			
	if(!lua.isValid()) {
		return;
	}
	
	// convert and store
	touchEvent.copyFromArgs(touch);
	
	try {
		luabind::call_function<bool>(lua, "touchCancelled", boost::ref(touchEvent));
	}
	catch(exception& e) {
		//ofLogWarning() << "ScriptEngine::touchCancelled: Caught exception: " << e.what();
	}
}

//--------------------------------------------------------------
void ScriptEngine::print(const string& message) {
	// append any leftovers in the writeBuffer
	ofLogNotice() << "LUA: " << writeBuffer << message;
	Global::instance().gui.console.addLine("LUA: " + writeBuffer + message);
	writeBuffer.clear();
}

//--------------------------------------------------------------
void ScriptEngine::write(const string& message) {
	// add to buffer, flush on each newline
	for(int i = 0; i < message.length(); ++i) {
		if(message[i] != '\n') {
			writeBuffer.push_back(message[i]);
		}
		else {
			ofLog() << "LUA: " << writeBuffer;
			Global::instance().gui.console.addLine("LUA: " + writeBuffer);
			writeBuffer.clear();
		}
	}
}

// PRIVATE
//--------------------------------------------------------------
void ScriptEngine::errorReceived(string& msg) {
	ofLogWarning() << "ScriptEngine: " << msg;
	Global::instance().setError(msg);
}
