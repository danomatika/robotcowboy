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

#include "ofEvents.h"

/// wrapper around ofTouchEventArgs to add copy constructor/operator
class TouchEvent : public ofTouchEventArgs {

	public:
		
		TouchEvent() {
			type = ofTouchEventArgs::down;
			this->id = -1;
			x = 0;
			y = 0;
			numTouches = 0;
			width = 0;
			height = 0;
			angle = 0;
			minoraxis = 0;
			majoraxis = 0;
			pressure =0;
			xspeed = 0;
			yspeed = 0;
			xaccel = 0;
			yaccel = 0;
		}
		
		TouchEvent(const TouchEvent& other) {
			copy(other);
		}
		
		TouchEvent& operator=(const TouchEvent& other) {
			return copy(other);
		}
		
		/// for operator= and copy constructor
		TouchEvent& copy(const TouchEvent& other) {
			type = other.type;
			this->id = other.id;
			x = other.x;
			y = other.y;
			numTouches = other.numTouches;
			width = other.width;
			height = other.height;
			angle = other.angle;
			minoraxis = other.minoraxis;
			majoraxis = other.majoraxis;
			pressure = other.pressure;
			xspeed = other.xspeed;
			yspeed = other.yspeed;
			xaccel = other.xaccel;
			yaccel = other.yaccel;
			return *this;
		}
		
		/// copy in ofTouchEventArgs data
		TouchEvent& copyFromArgs(const ofTouchEventArgs& other) {
			type = other.type;
			this->id = other.id;
			x = other.x;
			y = other.y;
			numTouches = other.numTouches;
			width = other.width;
			height = other.height;
			angle = other.angle;
			minoraxis = other.minoraxis;
			majoraxis = other.majoraxis;
			pressure = other.pressure;
			xspeed = other.xspeed;
			yspeed = other.yspeed;
			xaccel = other.xaccel;
			yaccel = other.yaccel;
			return *this;
		}
};
