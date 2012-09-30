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

#include "ofxBox2d.h"

namespace lua {

class Physics {

	public:
	
		static void bind(ofxLua& lua) {
		
			using namespace luabind;
			
			module(lua, "physics")
			[
				///////////////////////////////
				/// \section Physics
				
				def("frameRate", &frameRate),
				def("wakeupShapes", &wakeupShapes),
				
				def("getWorld", &getWorld),
				def("numBodies", &numBodies),
				def("numJoints", &numJoints),
				
				def("setIterations", &setIterations),
				def("setGravity", (void(*)(ofPoint p)) &setGravity),
				def("setGravity", (void(*)(float,float)) &setGravity),
				def("getGravity", &getGravity),
				def("getGravityX", &getGravityX),
				def("getGravityY", &getGravityY),
				
				def("grabbing", &grabbing),
				def("noGrabbing", &noGrabbing),
				
				///////////////////////////////
				/// \section Shape
				
				class_<ofxBox2dBaseShape>("shape")
					.def(constructor<>())
					.def("isBody", &ofxBox2dBaseShape::isBody)
					.def("isFixed", &ofxBox2dBaseShape::isFixed)
					.def("isSleeping", &ofxBox2dBaseShape::isSleeping)
					.def("setPhysics", &ofxBox2dBaseShape::setPhysics)
//					.def("gravity", &Physics::shapeGravity)
//					.def("noGravity", &Physics::shapeNoGravity)
					.def_readwrite("density", &ofxBox2dBaseShape::density)
					.def_readwrite("bounce", &ofxBox2dBaseShape::bounce)
					.def_readwrite("friction", &ofxBox2dBaseShape::friction),
				
				///////////////////////////////
				/// \section Circle
				
				class_<ofxBox2dCircle>("circle")
					.def(constructor<>())
//					.def("setup", (void(ofxBox2d::*)(ofPoint&,float)) &Physics::circleSetup)
//					.def("setup", (void(ofxBox2d::*)(float,float,float)) &Physics::circleSetup)
					.def("draw", &ofxBox2d::draw)
//					.def("addAttractionPoint", (void(ofxBox2d::*)(ofPoint,float)) &Physics::circleAddAttractionPoint)
//					.def("addAttractionPoint", (void(ofxBox2d::*)(float,float,float)) &ofxBox2d::addAttractionPoint)
//					.def("addRepulsionPoint", (void(ofxBox2d::*)(ofPoint,float)) &Physics::circleAddRepulsionPoint)
//					.def("addRepulsionPoint", (void(ofxBox2d::*)(float,float,float)) &ofxBox2dCirlce::addRepulsionPoint)
					.property("radius", &ofxBox2dCircle::getRadius, &ofxBox2dCircle::setRadius)
				
			];
		}
		
		/// \section Function & Object Wrappers

		//static void init() {Global::instance().physics.box2d.init();}
		
		static void frameRate(float fps) {
			Global::instance().physics.box2d.setFPS(fps);
		}
		static void wakeupShapes() {
			Global::instance().physics.box2d.wakeupShapes();
		}
		
		static b2World& getWorld() {
			return *Global::instance().physics.box2d.getWorld();
		}
		static int numBodies() {
			return Global::instance().physics.box2d.getBodyCount();
		}
		static int numJoints() {
			return Global::instance().physics.box2d.getJointCount();
		}
		
		static void setIterations(int velTimes, int posTimes) {
			Global::instance().physics.box2d.setIterations(velTimes, posTimes);
		}
		static void setGravity(ofPoint p) {
			Global::instance().physics.box2d.setGravity(p);
		}
		static void setGravity(float x, float y) {
			Global::instance().physics.box2d.setGravity(x, y);
		}
		static ofPoint getGravity() {
			return Global::instance().physics.box2d.getGravity();
		}
		static float getGravityX() {
			return Global::instance().physics.box2d.getGravity().x;
		}
		static float getGravityY() {
			return Global::instance().physics.box2d.getGravity().y;
		}
		
		static void grabbing() {
			Global::instance().physics.box2d.enableGrabbing();
		}
		static void noGrabbing() {
			Global::instance().physics.box2d.disableGrabbing();
		}
};

} // namespace
