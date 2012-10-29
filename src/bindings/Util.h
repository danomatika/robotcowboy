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

#include "ofxAppUtils.h"

namespace lua {

class Util {

	public:
	
		static void bind(ofxLua& lua) {
		
			using namespace luabind;
			
			module(lua, "util")
			[
				///////////////////////////////
				/// \section Util
				
				// TODO: needed?
				//def("getUnixTime", &ofGetUnixTime),
				//def("getSystemTime", &ofGetSystemTime),
				//def("getSystemTimeMicros", &ofGetSystemTimeMicros),
				
				def("timestampString", (string(*)(void)) &ofGetTimestampString),
				def("timestampString", (string(*)(string)) &ofGetTimestampString),

				def("second", &ofGetSeconds),
				def("minute", &ofGetMinutes),
				def("hour", &ofGetHours),
				def("day", &ofGetDay),
				//def("weekday", &ofGetWeekday), // needed?
				def("month", &ofGetMonth),
				def("year", &ofGetYear),
				
				def("saveScreen", &ofSaveScreen),
				def("saveFrame", &ofSaveFrame),
				
				///////////////////////////////
				/// \section Timer
				
				class_<ofxTimer>("Timer")
					.def(constructor<>())
					.def(constructor<unsigned int>())
					.def("set", &ofxTimer::set)
					.def("setAlarm", &ofxTimer::setAlarm)
					.def("resetAlarm", &ofxTimer::resetAlarm)
					.def("alarm", &ofxTimer::alarm)
					.def("getDiff", &ofxTimer::getDiff)
					.def("getDiffN", &ofxTimer::set)
			];
		}
		
		/// \section Function & Object Wrappers
};

} // namespace
