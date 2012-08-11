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
#include "ScriptEngine.h"

#include "ofxOsc.h"

#include "Global.h"
#include "LuaWrapper.h"

//--------------------------------------------------------------
ScriptEngine::ScriptEngine() {
	currentScript = "";
	errorOcurred = false;
	errorMsg = "";
	
	writeBuffer = "";
	
	sendsOscOut = false;
	
	lua.addListener(*this);
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
	errorOcurred = false;
	errorMsg = "";
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
	lua.bind<LuaWrapper>();
	currentScript = script;
	lua.doScript(bootScript);
	return lua.doScript(script);
}

//--------------------------------------------------------------
bool ScriptEngine::reloadScript() {
	ofLogVerbose() << "ScriptEngine: reloading script \"" << ofFilePath::getFileName(currentScript) << "\"";
	lua.clear();
	errorOcurred = false;
	errorMsg = "";
	if(!setup())
		return false;
	lua.bind<LuaWrapper>();
	lua.doScript(bootScript);
	return lua.doScript(currentScript);
}

//--------------------------------------------------------------
// calling lua functions with objects:
// http://www.nuclex.org/articles/cxx/1-quick-introduction-to-luabind
void ScriptEngine::sendOsc(ofxOscMessage& msg) {
	if(!lua.isValid())
		return;
	try {
		luabind::call_function<bool>(lua, "oscReceived", boost::ref(msg));
	}
	catch(exception& e) {
		ofLogWarning() << "ScriptEngine::sendOsc: Caught exception: " << e.what();
	}
}

//--------------------------------------------------------------
void ScriptEngine::print(const std::string& message) {
	// append any leftovers in the writeBuffer
	ofLogNotice() << "LUA: " << writeBuffer << message;
	Global::instance().gui.console.addLine("LUA: " + writeBuffer + message);
	writeBuffer.clear();
}

//--------------------------------------------------------------
void ScriptEngine::write(const std::string& message) {
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

//--------------------------------------------------------------
void ScriptEngine::errorReceived(const std::string& msg) {
	ofLogWarning() << "ScriptEngine: Got an error: " << msg;
	errorOcurred = true;
	errorMsg = msg;
}
