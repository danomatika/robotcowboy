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
#include <luabind/operator.hpp>

namespace lua {

// so named to avoid pd compilation issues due to math.h include ...
// and maybe a nod to a certian island nation
class Maths {

	public:
	
		static void bind(ofxLua& lua) {
		
			using namespace luabind;
			
			module(lua, "math")
			[
				///////////////////////////////
				/// \section Math
				
				def("nextPow2", &ofNextPow2),
				
				def("seedRandom", (void(*)(void)) &ofSeedRandom),
				def("seedRandom", (void(*)(int)) &ofSeedRandom),
				def("random", (float(*)(float)) &ofRandom),
				def("random", (float(*)(float,float)) &ofRandom),
				def("randomf", (float(*)(void)) &ofRandomf),
				def("randomuf", (float(*)(void)) &ofRandomuf),
				
				def("normalize", &ofNormalize),
				def("map", (float(*)(float,float,float,float,float)) &map5),
				def("map", (float(*)(float,float,float,float,float,bool)) &ofMap),
				def("clamp", &ofClamp),
				def("lerp", &ofLerp),
				def("dist", &ofDist),
				def("distSquared", &ofDistSquared),
				def("sign", &ofSign),
				def("inRange", &ofInRange),
				
				def("degrees", &ofRadToDeg),
				def("radians", &ofDegToRad),
				def("lerpDegrees", &ofLerpDegrees),
				def("lerpRadians", &ofLerpRadians),
				def("angleDifferenceDegrees", &ofAngleDifferenceDegrees),
				def("angleDifferenceRadians", &ofAngleDifferenceRadians),
				// TODO: missing implementation in CPP file, should be fixed in next PF release
				//def("angleSumRadians", (float(*)(float,float)) &ofAngleSumRadians),
				def("wrapRadians", &ofWrapRadians),
				def("wrapDegrees", &ofWrapDegrees),
				
				def("randomWidth", &ofRandomWidth),
				def("randomHeight", &ofRandomHeight),
				
				def("noise", (float(*)(float)) &ofNoise),
				def("noise", (float(*)(float,float)) &ofNoise),
				def("noise", (float(*)(float,float,float)) &ofNoise),
				def("noise", (float(*)(float,float,float,float)) &ofNoise),
				
				def("signedNoise", (float(*)(float)) &ofSignedNoise),
				def("signedNoise", (float(*)(float,float)) &ofSignedNoise),
				def("signedNoise", (float(*)(float,float,float)) &ofSignedNoise),
				def("signedNoise", (float(*)(float,float,float,float)) &ofSignedNoise),
				
				// TODO: needs vector
				//def("insidePoly", (bool(*)(float,float,const vector<ofPoint>&)) &ofInsidePoly),
				//def("insidePoly", (bool(*)(const ofPoint&,const vector<ofPoint>&)) &ofInsidePoly),
				
				def("lineSegmentIntersection", (bool(*)(ofPoint,ofPoint,ofPoint,ofPoint,ofPoint)) &ofLineSegmentIntersection),
				
				def("bezierPoint", (ofPoint(*)(ofPoint,ofPoint,ofPoint,ofPoint,float)) &ofBezierPoint),
				def("curvePoint", (ofPoint(*)(ofPoint,ofPoint,ofPoint,ofPoint,float)) &ofCurvePoint),
				def("bezierTangent", (ofPoint(*)(ofPoint,ofPoint,ofPoint,ofPoint,float)) &ofBezierTangent),
				def("curveTangent", (ofPoint(*)(ofPoint,ofPoint,ofPoint,ofPoint,float)) &ofCurveTangent),
				
				
				///////////////////////////////
				/// \section Point
				
				class_<ofPoint>("Point")
					.def(constructor<>())
					.def(constructor<const ofPoint&>())
					.def(constructor<float,float>())
					.def(constructor<float,float,float>())
					.def("set", &pointSet2)
					.def("set", (void(ofPoint::*)(const ofPoint&)) &ofPoint::set)
					.def("set", (void(ofPoint::*)(float,float,float)) &ofPoint::set)
					.def(self == other<const ofPoint>())
					.def("match", &pointMatch1)
					.def("match", (bool(ofPoint::*)(const ofPoint&,float)) &ofPoint::match)
					.def("isAligned", &pointIsAligned1)
					.def("isAligned", (bool(ofPoint::*)(const ofPoint&,float)) &ofPoint::isAligned)
					.def("isAlignedRad", &pointIsAlignedRad1)
					.def("isAlignedRad", &pointIsAlignedRad2)
					.def("align", &pointAlign1)
					.def("align", (bool(ofPoint::*)(const ofPoint&,float)) &ofPoint::align)
					.def("alignRad", &pointAlignRad1)
					.def("alignRad", (bool(ofPoint::*)(const ofPoint&,float)) &ofPoint::alignRad)
					.def(self == other<const ofPoint&>())
					.def(self + other<const ofPoint&>())
					.def(self - other<const ofPoint&>())
					.def(self * other<const ofPoint&>())
					.def(self / other<const ofPoint&>())
					.def(self + other<const float>())
					.def(self - other<const float>())
					.def(self * other<const float>())
					.def(self / other<const float>())
					.def("getScaled", (ofPoint(ofPoint::*)(const float)) &ofPoint::getScaled)
					.def("scale", (ofPoint&(ofPoint::*)(const float)) &ofPoint::scale)
					.def("getRotated", &pointGetRotated3)
					.def("getRotatedRad", &pointGetRotatedRad3)
					.def("rotate", (ofPoint&(ofPoint::*)(float,float,float)) &ofPoint::rotate)
					.def("rotateRad", (ofPoint&(ofPoint::*)(float,float,float)) &ofPoint::rotateRad)
					.def("getRotated", &pointGetRotated2)
					.def("getRotatedRad", &pointGetRotatedRad2)
					.def("rotate", (ofPoint&(ofPoint::*)(float,const ofPoint&)) &ofPoint::rotate)
					.def("rotateRad", (ofPoint&(ofPoint::*)(float,const ofPoint&)) &ofPoint::rotateRad)
					.def("getRotated", &pointGetRotated3p)
					.def("getRotatedRad", &pointGetRotatedRad3p)
					.def("rotate", (ofPoint&(ofPoint::*)(float,const ofPoint&,const ofPoint&)) &ofPoint::rotate)
					.def("rotateRad", (ofPoint&(ofPoint::*)(float,const ofPoint&,const ofPoint&)) &ofPoint::rotateRad)
					.def("getMapped", (ofPoint(ofPoint::*)(const ofPoint&,const ofPoint&,const ofPoint&,const ofPoint&)) &ofPoint::getMapped)
					.def("map", (ofPoint&(ofPoint::*)(const ofPoint&,const ofPoint&,const ofPoint&,const ofPoint&)) &ofPoint::map)
					.def("distance", (float(ofPoint::*)(const ofPoint&)) &ofPoint::distance)
					.def("squareDistance", (float(ofPoint::*)(const ofPoint&)) &ofPoint::squareDistance)
					.def("getInterpolated", (ofPoint(ofPoint::*)(const ofPoint&,float)) &ofPoint::getInterpolated)
					.def("interpolate", (ofPoint&(ofPoint::*)(const ofPoint&,float)) &ofPoint::interpolate)
					.def("getMidPoint", (ofPoint(ofPoint::*)(const ofPoint&)) &ofPoint::getMiddle)
					.def("midPoint", (ofPoint&(ofPoint::*)(const ofPoint&)) &ofPoint::middle)
					//.def("average", (ofPoint&(ofPoint::*)(const ofPoint*,int)) &ofPoint::average) // TODO: takes point array
					.def("getNormalized", (ofPoint(ofPoint::*)(void)) &ofPoint::getNormalized)
					.def("normalize", (ofPoint&(ofPoint::*)(void)) &ofPoint::normalize)
					.def("getLimited", (ofPoint(ofPoint::*)(float)) &ofPoint::getLimited)
					.def("limit", (ofPoint&(ofPoint::*)(float)) &ofPoint::limit)
					.def("getCrossed", (ofPoint(ofPoint::*)(const ofPoint&)) &ofPoint::getCrossed)
					.def("cross", (ofPoint&(ofPoint::*)(const ofPoint&)) &ofPoint::cross)
					.def("getPerpendicular", (ofPoint(ofPoint::*)(void)) &ofPoint::getPerpendicular)
					.def("perpendicular", (ofPoint&(ofPoint::*)(void)) &ofPoint::perpendicular)
					.def("length", (float(ofPoint::*)(void)) &ofPoint::length)
					.def("squareLength", (float(ofPoint::*)(void)) &ofPoint::squareLength)
					.def("angle", (float(ofPoint::*)(const ofPoint&)) &ofPoint::angle)
					.def("angleRad", (float(ofPoint::*)(const ofPoint&)) &ofPoint::angleRad)
					.def("dot", (float(ofPoint::*)(const ofPoint&)) &ofPoint::dot)
					.def_readwrite("x", &ofPoint::x)
					.def_readwrite("y", &ofPoint::y)
					.def_readwrite("z", &ofPoint::z)
			];
		}
		
		/// \section Function & Object Wrappers
		
		/// math
		static float map5(float value, float inputMin, float inputMax, float outputMin, float outputMax) {
			return ofMap(value, inputMin, inputMax, outputMin, outputMax);
		}

		/// point
		static void pointSet2(ofPoint* point, float x, float y) {
			point->set(x, y);
		}
		static float pointMatch1(ofPoint* point, const ofPoint& other) {
			return point->match(other);
		}
		static float pointIsAligned1(ofPoint* point, const ofPoint& other) {
			return point->isAligned(other);
		}
		static float pointIsAlignedRad1(ofPoint* point, const ofPoint& other) {
			return point->isAlignedRad(other);
		}
		static float pointIsAlignedRad2(ofPoint* point, const ofPoint& other, float tolerance) {
			return point->isAlignedRad(other, tolerance);
		}
		static float pointAlign1(ofPoint* point, const ofPoint& other) {
			return point->align(other);
		}
		static float pointAlignRad1(ofPoint* point, const ofPoint& other) {
			return point->alignRad(other);
		}
		
		/// point const wrappers
		static ofPoint pointGetRotated2(ofPoint* point, float angle, const ofPoint& axis) {
			return point->getRotated(angle, axis);
		}
		static ofPoint pointGetRotatedRad2(ofPoint* point, float angle, const ofPoint& axis) {
			return point->getRotatedRad(angle, axis);
		}
		static ofPoint pointGetRotated3(ofPoint* point, float ax, float ay, float az) {
			return point->getRotated(ax, ay, az);
		}
		static ofPoint pointGetRotatedRad3(ofPoint* point, float ax, float ay, float az) {
			return point->getRotatedRad(ax, ay, az);
		}
		static ofPoint pointGetRotated3p(ofPoint* point, float angle, const ofPoint& pivot,const ofPoint& axis) {
			return point->getRotated(angle, pivot, axis);
		}
		static ofPoint pointGetRotatedRad3p(ofPoint* point, float angle, const ofPoint& pivot,const ofPoint& axis) {
			return point->getRotatedRad(angle, pivot, axis);
		}
};

} // namespace
