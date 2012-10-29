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

#include "../Global.h"

#include "App.h"
#include "Audio.h"
#include "Graphics.h"
#include "Maths.h"
#include "Osc.h"
#include "Physics.h"
#include "Util.h"

//
// naming guidelines
//
// global:
// * getters drop "get" prepend -> getWidth() becomes width()
// * setters keep "set" prepend -> setWidth() stays setWidth()
// * binary mode setters use "name" / "noName" -> smooth() / noSmooth()
// * try to associate enums with pertinent classes -> app.mouse.LEFT
//
// class binding:
// * class names start with upper case letters -> math.Point not math.point
// * getters keep "get" prepend, unless they are for simple sizes/lengths/nums
//   -> pixels.size() & msg.numArgs() instead of pixels.getSize() msg.getNumArgs()
// * setters keep "set" prepend
// * replace getter/setter pairs with a property *only* if that variable is a
//   simple class member that dosen't need to be computed/set 
//
class Bindings {

	public:
	
		/// static function called when binding
		static void bind(ofxLua& lua) {
		
			using namespace luabind;
			
			lua::App::bind(lua);
			lua::Audio::bind(lua);
			lua::Graphics::bind(lua);
			lua::Maths::bind(lua);
			lua::Osc::bind(lua);
			lua::Physics::bind(lua);
			lua::Util::bind(lua);

			/// rc-specifics
			module(lua, "rc")
			[
				/// to override lua print and write functions
				def("print", &print),
				def("write", &write)
			];
		}
		
		/// \section Function & Object Wrappers
		
		/// console io
		static void print(const std::string& message) {
			Global::instance().scriptEngine.print(message);
		}
		
		static void write(const std::string& message) {
			Global::instance().scriptEngine.write(message);
		}
};
