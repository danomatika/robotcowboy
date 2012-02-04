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
}

//--------------------------------------------------------------
bool ScriptEngine::setup() {
	if(!lua.init()) {
		ofLog(OF_LOG_ERROR, "ScriptEngine: Could not init lua");
		return false;
	}
	return true;
}

//--------------------------------------------------------------
void ScriptEngine::clear() {
	lua.clear();
	currentScript = "";
}

//--------------------------------------------------------------
bool ScriptEngine::loadScript(string script) {
	cout << "ScriptEngine: loading script \""
		 << script << "\"" << endl;
	lua.scriptExit();
	lua.clear();
	lua.init();
	lua.bind<LuaWrapper>();
	bool ret = lua.doScript(script);
	if(ret)
		currentScript = script;
	return ret;
}

//--------------------------------------------------------------
bool ScriptEngine::reloadScript() {
	cout << "ScriptEngine: reloading script \""
		 << currentScript << "\"" << endl;
	lua.scriptExit();
	lua.clear();
	lua.init();
	lua.bind<LuaWrapper>();
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
		cout << "ScriptEngine::sendOsc: Caught exception: " << e.what() << endl;
	}
}

//--------------------------------------------------------------
void ScriptEngine::errorReceived(const std::string& msg) {
	cout << "ScriptEngine: Got an error: " << msg << endl;
}