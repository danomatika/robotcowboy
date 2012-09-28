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

#include "Global.h"

#include "ofxOsc.h"
#include "ofxPd.h"
#include "ofxAppUtils.h"
#include "ofxUI.h"

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
// * getters keep "get" prepend, unless they are for simple sizes/lengths/nums
//   -> pixels.size() & msg.numArgs() instead of pixels.getSize() msg.getNumArgs()
// * setters keep "set" prepend
// * replace getter/setter pairs with a property *only* if that variable is a
//   simple class member that dosen't need to be computed/set 
//
class Bindings {

	private:
	
		/// dummy classes for empty class enums
		struct BlendMode {};
		struct MouseVars {};
		struct KeyboardVars {};

	public:
	
		/// static function called when binding
		static void bind(ofxLua& lua) {
		
			using namespace luabind;
						
			module(lua, "graphics")
			[
				///////////////////////////////
				/// \section Graphics
				
				/// openGL wrappers
				def("pushMatrix", &ofPushMatrix),
				def("popMatrix", &ofPopMatrix),
				def("translate", (void(*)(float,float,float)) &ofTranslate),
				def("scale", &ofScale),
				def("rotate", (void(*)(float,float,float,float)) &ofRotate),
				def("rotateX", &ofRotateX),
				def("rotateY", &ofRotateY),
				def("rotateZ", &ofRotateZ),
				def("rotate", (void(*)(float)) &ofRotate),
				
				/// drawing detail
				def("setCircleResolution", &ofSetCircleResolution),
				def("setSphereResolution", &ofSetSphereResolution),
				def("setCurveResolution", &ofSetCurveResolution),
				
				/// drawing options
				def("fill", &ofFill),
				def("noFill", &ofNoFill),
				def("setLineWidth", &ofSetLineWidth),
				
				/// current color
				def("setColor", (void(*)(int)) &ofSetColor),
				def("setColor", (void(*)(int,int,int)) &ofSetColor),
				def("setColor", (void(*)(int,int,int,int)) &ofSetColor),
				def("setColor", (void(*)(const ofColor&)) &ofSetColor),
				def("setColor", (void(*)(const ofColor&,int)) &ofSetColor),
				def("setHexColor", (void(*)(int)) &ofSetHexColor),
				
				/// blending
				def("blendMode", &ofEnableBlendMode),
				def("noBlending", &ofDisableBlendMode),
				class_<BlendMode>("blendMode")
					.enum_("mode")
					[
						value("DISABLED", OF_BLENDMODE_DISABLED),
						value("ALPHA", OF_BLENDMODE_ALPHA),
						value("ADD", OF_BLENDMODE_ADD),
						value("SUBTRACT", OF_BLENDMODE_SUBTRACT),
						value("MULTIPLY", OF_BLENDMODE_MULTIPLY),
						value("SCREEN", OF_BLENDMODE_SCREEN)
					],
				
				/// point sprites
				def("pointSprites", &ofEnablePointSprites),
				def("noPointSprites", &ofDisablePointSprites),
				
				/// transparency
				def("alphaBlend", &ofEnableAlphaBlending),
				def("noAlphaBlend", &ofDisableAlphaBlending),
				
				/// smooth
				def("smooth", &ofEnableSmoothing),
				def("noSmooth", &ofDisableSmoothing),
				
				/// drawing style
                def("getStyle", &ofGetStyle),
				def("setStyle", &ofSetStyle),
				def("pushStyle", &ofPushStyle),
				class_<ofStyle>("style")
					.def(constructor<>())
					.def_readwrite("color", &ofStyle::color)
					.def_readwrite("background", &ofStyle::bgColor)
					.def_readwrite("rectMode", &ofStyle::rectMode)
					.def_readwrite("polyMode", &ofStyle::polyMode)
					.def_readwrite("filled", &ofStyle::bFill)
					//.def_readwrite("bitmapMode", &ofStyle::drawBitmapMode) // TODO: not needed?
					.def_readwrite("blendMode", &ofStyle::blendingMode)
					.def_readwrite("smoothing", &ofStyle::smoothing)
					.def_readwrite("circleResolution", &ofStyle::circleResolution)
					.def_readwrite("sphereResolution", &ofStyle::sphereResolution)
					.def_readwrite("curveResolution", &ofStyle::curveResolution)
					.def_readwrite("lineWidth", &ofStyle::lineWidth),
				
				def("setRectMode", &ofSetRectMode),
				def("setPolyMode", &ofSetPolyMode),
				
				/// background
				def("background", (void(*)(int)) &graphicsBackground),
				def("background", (void(*)(int,int)) &ofBackground),
				def("background", (void(*)(int,int,int)) &graphicsBackground),
				def("background", (void(*)(int,int,int,int)) &ofBackground),
				def("background", (void(*)(const ofColor&)) &ofBackground),
				def("backgroundHex", (void(*)(int)) &graphicsBackground),
				def("backgroundHex", (void(*)(int,int)) &ofBackgroundHex),
				
				def("autoBackground", &graphicsAutoBackground),
				def("noAutoBackground", &graphicsNoAutoBackground),
				def("usingAutoBackground", &ofbClearBg),
				
				def("clear", (void(*)(float)) &graphicsClear),
				def("clear", (void(*)(float,float)) &ofClear),
				def("clear", (void(*)(float,float,float,float)) &ofClear),
				def("clearAlpha", &ofClearAlpha),
						
				/// geometry
				def("line", (void(*)(float,float,float,float)) &ofLine),
				def("line", (void(*)(float,float,float,float,float,float)) &ofLine),
				def("line", (void(*)(const ofPoint&,const ofPoint&)) &ofLine),
				
				def("rect", (void(*)(const ofRectangle&)) &ofRect),
				def("rect", (void(*)(const ofPoint&,float,float)) &ofRect),
				def("rect", (void(*)(float,float,float,float)) &ofRect),
				def("rect", (void(*)(float,float,float,float,float)) &ofRect),
				
				def("triangle",	(void(*)(const ofPoint&,const ofPoint&,const ofPoint&)) &ofTriangle),
				def("triangle",	(void(*)(float,float,float,float,float,float)) &ofTriangle),
				def("triangle",	(void(*)(float,float,float,float,float,float,float,float,float)) &ofTriangle),
				
				def("circle", (void(*)(float,float,float)) &ofCircle),
				def("circle", (void(*)(float,float,float,float)) &ofCircle),
				def("circle", (void(*)(const ofPoint&,float)) &ofCircle),
				
				def("ellipse", (void(*)(const ofPoint&,float,float)) &ofEllipse),
				def("ellipse", (void(*)(float,float,float,float)) &ofEllipse),
				def("ellipse", (void(*)(float,float,float,float,float)) &ofEllipse),
				
				def("curve", (void(*)(float,float,float,float,float,float,float,float)) &ofCurve),
				def("curve", (void(*)(const ofPoint&,const ofPoint&,const ofPoint&,const ofPoint&)) &graphicsCurve),
				
				def("bezier", (void(*)(float,float,float,float,float,float,float,float)) &ofBezier),
				def("bezier", (void(*)(const ofPoint&,const ofPoint&,const ofPoint&,const ofPoint&)) &graphicsBezier),
				
				/// polygons
				def("beginShape", &ofBeginShape),
				def("vertex",	(void(*)(ofPoint&)) &ofVertex),
				def("vertex",	(void(*)(float,float)) &ofVertex),
				def("vertex",	(void(*)(float,float,float)) &ofVertex),
				//def("vertices", &vertices), // TODO: needs vector
				
				def("curveVertex", (void(*)(ofPoint&)) &ofCurveVertex),
				def("curveVertex", (void(*)(float,float)) &ofCurveVertex),
				//def("curveVertices", &curveVertices), // TODO: needs vector
				
				def("bezierVertex", (void(*)(const ofPoint&,const ofPoint&,const ofPoint&)) &ofBezierVertex),
				def("bezierVertex", (void(*)(float,float,float,float,float,float)) &ofBezierVertex),
				def("bezierVertex", (void(*)(float,float,float,float,float,float,float,float,float)) &ofBezierVertex),
				//def("bezierVertices", &bezierVertices), // TODO: needs vector
				
				def("endShape",	(void(*)(void)) &graphicsEndShape),
				def("endShape",	(void(*)(bool)) &ofEndShape),
				def("nextContour", (void(*)(void)) &graphicsNextContour),
				def("nextContour", (void(*)(bool)) &ofNextContour),
				
				/// 3d primitives
				def("sphere", (void(*)(float)) &ofSphere),
				def("sphere", (void(*)(const ofPoint&,float)) &ofSphere),
				def("sphere", (void(*)(float,float,float,float)) &ofSphere),
				def("sphere", (void(*)(float,float,float,float)) &ofSphere),
				
				def("box", (void(*)(float)) &ofBox),
				def("box", (void(*)(const ofPoint&,float)) &ofBox),
				def("box", (void(*)(float,float,float,float)) &ofBox),
				def("box", (void(*)(float,float,float,float)) &ofBox),
				
				def("cone", (void(*)(float,float)) &ofCone),
				def("cone", (void(*)(const ofPoint&,float,float)) &ofCone),
				def("cone", (void(*)(float,float,float,float)) &ofCone),
				def("cone", (void(*)(float,float,float,float,float)) &ofCone),
				
				/// 3d utils
				def("axis", &ofDrawAxis),
				def("grid", &graphicsDrawGrid0),
				def("grid", &graphicsDrawGrid3),
				def("grid", &ofDrawGrid),
				def("gridPlane", &graphicsDrawGridPlane1),
				def("gridPlane", &ofDrawGridPlane),
				def("arrow", &graphicsDrawArrow2),
				def("arrow", &ofDrawArrow),
				
				/// bitmapped text
				def("bitmapString", (void(*)(string,const ofPoint&)) &ofDrawBitmapString),
				def("bitmapString", (void(*)(string,float,float)) &ofDrawBitmapString),
				def("bitmapString", (void(*)(string,float,float,float)) &ofDrawBitmapString),
			
				///////////////////////////////
				/// \section PolyLine
				
				class_<ofPolyline>("polyline")
					.def(constructor<>())
					.def("clear", &ofPolyline::clear)
					.def("addVertex", (void(ofPolyline::*)(const ofPoint&)) &ofPolyline::addVertex)
					.def("addVertex", &polylineAddVertex2)
					.def("addVertex", (void(ofPolyline::*)(float,float,float)) &ofPolyline::addVertex)
					.def("lineTo", (void(ofPolyline::*)(const ofPoint&)) &ofPolyline::lineTo)
					.def("lineTo", &polylineLineTo2)
					.def("lineTo", (void(ofPolyline::*)(float,float,float)) &ofPolyline::lineTo)
					.def("arc", &polylineArcPoint5)
					.def("arc", (void(ofPolyline::*)(const ofPoint&,float,float,float,float,int)) &ofPolyline::arc)
					.def("arc", &polylineArcPoint6)
					.def("arc", (void(ofPolyline::*)(const ofPoint&,float,float,float,float,bool,int)) &ofPolyline::arc)
					.def("arc", &polylineArcFloat6)
					.def("arc", (void(ofPolyline::*)(float,float,float,float,float,float,int)) &ofPolyline::arc)
					.def("arc", &polylineArcFloat7)
					.def("arc", (void(ofPolyline::*)(float,float,float,float,float,float,float,int)) &ofPolyline::arc)
					.def("arcNegative", (void(ofPolyline::*)(const ofPoint&,float,float,float,float,int)) &ofPolyline::arcNegative)
					.def("arcNegative", &polylineArcNegativePoint5)
					.def("arcNegative", (void(ofPolyline::*)(float,float,float,float,float,float,int)) &ofPolyline::arcNegative)
					.def("arcNegative", &polylineArcNegativeFloat6)
					.def("arcNegative", (void(ofPolyline::*)(float,float,float,float,float,float,float,int)) &ofPolyline::arcNegative)
					.def("arcNegative", &polylineArcNegativeFloat7)
					.def("curveTo", &polyLineCurveToPoint1)
					.def("curveTo", (void(ofPolyline::*)(const ofPoint&,int)) &ofPolyline::curveTo)
					.def("curveTo", &polyLineCurveToFloat2)
					.def("curveTo", &polyLineCurveToFloat3)
					.def("curveTo", (void(ofPolyline::*)(float,float,float,int)) &ofPolyline::curveTo)
					.def("bezierTo", &polylineBezierToPoint3)
					.def("bezierTo", (void(ofPolyline::*)(const ofPoint&,const ofPoint&,const ofPoint&, int)) &ofPolyline::bezierTo)
					.def("bezierTo", &polylineBezierToFloat6)
					.def("bezierTo", (void(ofPolyline::*)(float,float,float,float,float,float,int)) &ofPolyline::bezierTo)
					.def("bezierTo", &polylineBezierToFloat9)
					.def("bezierTo", (void(ofPolyline::*)(float,float,float,float,float,float,float,float,float,int)) &ofPolyline::bezierTo)
					.def("quadBezierTo", &polylineQuadBezierToPoint3)
					.def("quadBezierTo", (void(ofPolyline::*)(const ofPoint&,const ofPoint&,const ofPoint&, int)) &ofPolyline::quadBezierTo)
					.def("quadBezierTo", &polylineQuadBezierToFloat6)
					.def("quadBezierTo", (void(ofPolyline::*)(float,float,float,float,float,float,int)) &ofPolyline::quadBezierTo)
					.def("quadBezierTo", &polylineQuadBezierToFloat9)
					.def("quadBezierTo", (void(ofPolyline::*)(float,float,float,float,float,float,float,float,float,int)) &ofPolyline::quadBezierTo)
					.def("getSmoothed", &polylineGetSmoothed1)
					.def("getSmoothed", (void(ofPolyline::*)(int,float)) &ofPolyline::getSmoothed)
					.def("getResampledBySpacing", &ofPolyline::getResampledBySpacing)
					.def("getResampledByCount", &ofPolyline::getResampledByCount)
					.def("getBoundingBox", &ofPolyline::getBoundingBox)
					.def("getClosestPoint", &polylineGetClosestPoint1)
					.def("getClosestPointIndex", &polylineGetClosestPointIndex)
					.def("inside", (bool(ofPolyline::*)(const ofPoint&)) &ofPolyline::inside)
					.def("inside", (bool(ofPolyline::*)(float,float)) &ofPolyline::inside)
					//.def("inside", (bool(ofPolyline::*)(const ofPoint&,const ofPolyline&)) &ofPolyline::inside) // TODO: these are static
					//.def("inside", (bool(ofPolyline::*)(float,float,const ofPolyline&)) &ofPolyline::inside) // TODO: static
					.def("simplify", &polylineSimplify0)
					.def("simplify", &ofPolyline::simplify)
					.def("getPoint", &polylineGetPoint)
					.def("setPoint", &polylineSetPoint)
					.def("resize", &ofPolyline::resize) // needed? too low level?
					.def("setClosed", &ofPolyline::setClosed)
					.def("isClosed", &ofPolyline::isClosed)
					.def("close", &ofPolyline::close)
					.def("hasChanged", &ofPolyline::hasChanged)
					//.def("getVertices", &ofPolyline::getVertices) // TODO: returns vector
					.def("getPerimeter", &ofPolyline::getPerimeter)
					.def("getArea", &ofPolyline::getArea)
					.def("getCentroid", &ofPolyline::getCentroid2D)
					.def("draw", &ofPolyline::draw)
					.def_readonly("size", &ofPolyline::size)
					.enum_("windingMode")
					[	
						value("ODD", OF_POLY_WINDING_ODD),
						value("NONZERO", OF_POLY_WINDING_NONZERO),
						value("POSITIVE", OF_POLY_WINDING_POSITIVE),
						value("NEGATIVE", OF_POLY_WINDING_NEGATIVE),
						value("ABS_GEQ_TWO", OF_POLY_WINDING_ABS_GEQ_TWO)
					],
					
					// TODO: make these globals, although they are class functions
					//def("insidePolyline", (bool(ofPolyline::*)(const ofPoint&,const ofPolyline&)) &ofPolyline::inside),
					//def("insidePolyline", (bool(ofPolyline::*)(float,float,const ofPolyline&)) &ofPolyline::inside),
					
				///////////////////////////////
				/// \section Path
				
				class_<ofPath>("path")
					.def(constructor<>())
					.def("clear", &ofPath::clear)
					.def("newSubPath", &ofPath::newSubPath)
					.def("close", &ofPath::close)
					.def("lineTo", (void(ofPath::*)(const ofPoint&)) &ofPath::lineTo)
					.def("lineTo", (void(ofPath::*)(float,float)) &ofPath::lineTo)
					.def("lineTo", (void(ofPath::*)(float,float,float)) &ofPath::lineTo)
					.def("moveTo", &pathMoveTo1)
					.def("moveTo", &pathMoveTo2)
					.def("moveTo", (void(ofPath::*)(float,float,float)) &ofPath::moveTo)
					.def("curveTo", &pathCurveTo1)
					.def("curveTo",  (void(ofPath::*)(float,float)) &ofPath::curveTo)
					.def("curveTo", (void(ofPath::*)(float,float,float)) &ofPath::curveTo)
					.def("bezierTo", (void(ofPath::*)(const ofPoint&,const ofPoint&,const ofPoint&)) &ofPath::bezierTo)
					.def("bezierTo", (void(ofPath::*)(float,float,float,float,float,float)) &ofPath::bezierTo)
					.def("bezierTo", (void(ofPath::*)(float,float,float,float,float,float,float,float,float)) &ofPath::bezierTo)
					.def("quadBezierTo", (void(ofPath::*)(const ofPoint&,const ofPoint&,const ofPoint&)) &ofPath::quadBezierTo)
					.def("quadBezierTo", (void(ofPath::*)(float,float,float,float,float,float)) &ofPath::quadBezierTo)
					.def("quadBezierTo", (void(ofPath::*)(float,float,float,float,float,float,float,float,float)) &ofPath::quadBezierTo)
					.def("arc", (void(ofPath::*)(const ofPoint&,float,float,float,float)) &ofPath::arc)
					.def("arc", (void(ofPath::*)(const ofPoint&,float,float,float,float,bool)) &ofPath::arc)
					.def("arc", (void(ofPath::*)(float,float,float,float,float,float)) &ofPath::arc)
					.def("arc", (void(ofPath::*)(float,float,float,float,float,float,float)) &ofPath::arc)
					.def("arcNegative", (void(ofPath::*)(const ofPoint&,float,float,float,float)) &ofPath::arcNegative)
					.def("arcNegative", (void(ofPath::*)(float,float,float,float,float,float)) &ofPath::arcNegative)
					.def("arcNegative", (void(ofPath::*)(float,float,float,float,float,float,float)) &ofPath::arcNegative)
					.def("setWindingMode", &ofPath::setPolyWindingMode)
					.def("getWindingMode", &ofPath::getWindingMode)
					.def("setFilled", &ofPath::setFilled)
					.def("isFilled", &ofPath::isFilled)
					//.def("setColor", &ofPath::setColor) // set fill/stroke
					//.def("setHexColor", &ofPath::setHexColor) // set fill/stroke
					.def("setFill", &ofPath::setFillColor)
					.def("setFillHex", &ofPath::setFillHexColor)
					.def("getFill", &ofPath::getFillColor)
					.def("setStroke", &ofPath::setStrokeColor)
					.def("setStrokeHex", &ofPath::setStrokeHexColor)
					.def("getStroke", &ofPath::getStrokeColor)
					.def("setStrokeWidth", &ofPath::setStrokeWidth)
					.def("getStrokeWidth", &ofPath::getStrokeWidth)
					.def("draw", (void(ofPath::*)(void)) &ofPath::draw)
					.def("draw", (void(ofPath::*)(float,float)) &ofPath::draw)
					//.def("getSubPaths", &ofPath::getSubPaths) // TODO: returns vector
					//.def("getSubPath", &pathGetSubPath) // TODO: returns ofSubPath ...
					//.def("setSubPath", &pathSetSubPath) // .. which is a primitive
					//.def("numSubPaths", &pathNumSubPaths) // that dosen't need bindings
					//.def("getOutline", &ofPath::getOutline) // TODO: returns vector
					.def("getOutline", &pathGetOutline)
					//.def("setOutline", &pathSetOutline) // TODO: no ofPolyline copy operator yet
					.def("numOutlines", &pathNumOutlines)
					.def("getMesh", &ofPath::getTessellation)
					.def("simplify", &pathSimplify0)
					.def("simplify", &ofPath::simplify)
					.def("update", &ofPath::flagShapeChanged)
					.def("setMode", &ofPath::setMode)
					.def("enableShapeColor", &pathEnableShapeColor)
					.def("disableShapeColor", &pathDisableShapeColor)
					.def("usingShapeColor", &ofPath::getUseShapeColor)
					.def("translate", &ofPath::translate)
					.def("translate", &pathTranslate2)
					.def("rotate", &pathRotate1)
					.def("rotateX", &pathRotateX)
					.def("rotateY", &pathRotateY)
					.def("rotateZ", &pathRotateZ)
					.def("rotate", &ofPath::rotate)
					.def("rotate", pathRotate4)
					.def("scale", &ofPath::scale)
					.property("curveResolution", &ofPath::getCurveResolution, &ofPath::setCurveResolution)
					.property("arcResolution", &ofPath::getArcResolution, &ofPath::setArcResolution)
					.enum_("mode")
					[
						value("PATHS", ofPath::PATHS),
						value("POLYLINES", ofPath::POLYLINES)
					],
			
				///////////////////////////////
				/// \section Mesh
				
				class_<ofMesh>("mesh")
					.def(constructor<>())
					//.def(constructor<ofPrimitiveMode,vector<ofPoint>&>()) // TODO: needs vector
					.def("clear", &ofMesh::clear)
					.def("getMode", &ofMesh::setMode)
					.def("setMode", &ofMesh::getMode)
					.def("enableAutoIndices", &ofMesh::setupIndicesAuto)
					.def("getVertex", &ofMesh::getVertex)
					.def("setVertex", &ofMesh::setVertex)
					.def("addVertex", (void(ofMesh::*)(const ofPoint&)) &ofMesh::addVertex)
					//.def("addVertices", (void(ofMesh::*)(const vector<ofPoint>&)) &ofMesh::addVertices) // TODO: needs vector
					.def("removeVertex", &ofMesh::removeVertex)
					.def("clearVertices", &ofMesh::clearVertices)
					.def("numVertices", &ofMesh::getNumVertices)
					.def("getNormal", &ofMesh::getNormal)
					.def("setNormal", &ofMesh::setNormal)
					.def("addNormal", (void(ofMesh::*)(const ofPoint&)) &ofMesh::addNormal)
					//.def("addNormal", (void(ofMesh::*)(const vector<ofPoint>&)) &ofMesh::addNormal) // TODO: needs vector
					.def("removeNormal", &ofMesh::removeNormal)
					.def("clearNormals", &ofMesh::clearNormals)
					.def("numNormals", &ofMesh::getNumNormals)
					.def("getColor", &ofMesh::getColor)
					.def("setColor", &ofMesh::setColor)
					.def("addColor", (void(ofMesh::*)(const ofPoint&)) &ofMesh::addColor)
					//.def("addColors", (void(ofMesh::*)(const vector<ofPoint>&)) &ofMesh::addColors) // TODO: needs vector
					.def("removeColor", &ofMesh::removeColor)
					.def("clearColors", &ofMesh::clearColors)
					.def("numColors", &ofMesh::getNumColors)
					.def("getTexCoord", &meshGetTexCoord) // need wrappers to convert
					.def("setTexCoord", &meshSetTexCoord) // between ofVec2f and ofPoint
					.def("addTexCoord", &meshAddTexCoord)
					//.def("addTexCoord", (void(ofMesh::*)(const vector<ofPoint>&)) &ofMesh::addTexCoord) // TODO: needs vector
					.def("removeTexCoord", &ofMesh::removeTexCoord)
					.def("clearTexCoords", &ofMesh::clearTexCoords)
					.def("numTexCoords", &ofMesh::getNumTexCoords)
					.def("getIndex", &ofMesh::getIndex)
					.def("setIndex", (int(ofMesh::*)(int,int)) &ofMesh::setIndex)
					.def("addIndex", (void(ofMesh::*)(const ofPoint&)) &ofMesh::addIndex)
					//.def("addIndices", (void(ofMesh::*)(const vector<ofPoint>&)) &ofMesh::addIndices) // TODO: needs vector
					.def("removeIndex", &ofMesh::removeIndex)
					.def("clearIndices", &ofMesh::clearIndices)
					.def("numIndices", &ofMesh::getNumIndices)
					.def("addTriangle", (void(ofMesh::*)(int,int,int)) &ofMesh::addTriangle)
					//.def("getFace", &ofMesh::getFace) // TODO: needs vector
					.def("getCentroid", &ofMesh::getCentroid)
					.def("setName", &ofMesh::setName)
					.def("haveVerticesChanged", &ofMesh::haveVertsChanged)
					.def("haveColorsChanged", &ofMesh::haveColorsChanged)
					.def("haveNormalsChanged", &ofMesh::haveNormalsChanged)
					.def("haveTexCoordsChanged", &ofMesh::haveTexCoordsChanged)
					.def("haveIndicesChanged", &ofMesh::haveIndicesChanged)
					.def("hasVertices", &ofMesh::hasVertices)
					.def("hasColors", &ofMesh::hasColors)
					.def("hasNormals", &ofMesh::hasNormals)
					.def("hasTexCoords", &ofMesh::hasTexCoords)
					.def("hasIndices", &ofMesh::hasIndices)
					.def("draw", (void(ofMesh::*)(void)) &ofMesh::draw)
					.def("drawFaces", &ofMesh::drawFaces)
					.def("drawVertices", &ofMesh::drawVertices)
					.def("drawWireframe", &ofMesh::drawWireframe)
					.def("loadMesh", &ofMesh::load)
					.def("load", &meshSave1)
					.def("saveMesh", &ofMesh::save)
					.def("enableColors", &ofMesh::enableColors)
					.def("enableTextures", &ofMesh::enableTextures)
					.def("enableNormals", &ofMesh::enableNormals)
					.def("disableColors", &ofMesh::disableColors)
					.def("disableTextures", &ofMesh::disableTextures)
					.def("edisableNormals", &ofMesh::disableNormals)
					.def("usingColors", &ofMesh::usingColors)
					.def("usingTextures", &ofMesh::usingTextures)
					.def("usingNormals", &ofMesh::usingNormals),
			
				///////////////////////////////
				/// \section Rectangle
				
				class_<ofRectangle>("rectangle")
					.def(constructor<>())
					.def(constructor<const ofRectangle&>())
					.def(constructor<ofPoint,float,float>())
					.def(constructor<float,float,float,float>())
					.def("set", (void(ofRectangle::*)(const ofRectangle&)) &ofRectangle::set)
					.def("set", (void(ofRectangle::*)(ofPoint,float,float)) &ofRectangle::set)
					.def("set", (void(ofRectangle::*)(float,float,float,float)) &ofRectangle::set)
					.def("setFromCenter", (void(ofRectangle::*)(ofPoint,float,float)) &ofRectangle::setFromCenter)
					.def("setFromCenter", (void(ofRectangle::*)(float,float,float,float)) &ofRectangle::setFromCenter)
					.def("getCenter", &ofRectangle::getCenter)
					.def("inside", (bool(ofRectangle::*)(ofPoint)) &ofRectangle::inside)
					.def("inside", (bool(ofRectangle::*)(float,float)) &ofRectangle::inside)
					.def(self == other<const ofRectangle&>())
					.def(self + other<const ofPoint&>())
					.def_readwrite("x", &ofRectangle::x)
					.def_readwrite("y", &ofRectangle::y)
					.def_readwrite("width", &ofRectangle::width)
					.def_readwrite("height", &ofRectangle::height)
                    .enum_("mode")
					[	
						value("CORNER", OF_RECTMODE_CORNER),
                        value("CENTER", OF_RECTMODE_CENTER)
					],
			
				///////////////////////////////
				/// \section Color
				
				class_<ofColor>("color")
					.def(constructor<>())
					.def(constructor<ofColor&>())
					.def(constructor<ofColor&,float>())
					.def(constructor<float>())
					.def(constructor<float,float>())
					.def(constructor<float,float,float>())
					.def(constructor<float,float,float,float>())
					.def("set", &colorSet1)
					.def("set", (void(ofColor::*)(float,float)) &ofColor::set)
					.def("set", &colorSet3)
					.def("set", (void(ofColor::*)(float,float,float,float)) &ofColor::set)
					.def("setHex", &ofColor::setHex)
					.def("getHex", &ofColor::getHex)
					.def("clamp", &ofColor::clamp)
					.def("invert", &ofColor::invert)
					.def("normalize", &ofColor::normalize)
					.def("lerp", &ofColor::lerp)
					.def("getClamped", &ofColor::getClamped)
					.def("getInverted", &ofColor::getInverted)
					.def("getNormalized", &ofColor::getNormalized)
					.def("getLerped", &ofColor::getLerped)
					.def("getHue", &ofColor::getHue)
					.def("getSaturation", &ofColor::getSaturation)
					.def("getBrightness", &ofColor::getBrightness)
					.def("getLightness", &ofColor::getLightness)
					//.def("getHsb", &ofColor::getHsb) // float refs , works in lua?
					.def("setHue", &ofColor::setHue)
					.def("setSaturation", &ofColor::setSaturation)
					.def("setBrightness", &ofColor::setBrightness)
					.def("setHsb", &colorSetHsb3)
					.def("setHsb", &ofColor::setHsb)
					.def(self == other<const ofColor&>())
					.def(self + other<const ofColor&>())
					.def(self - other<const ofColor&>())
					.def(self * other<const ofColor&>())
					.def(self / other<const ofColor&>())
					.def(self + other<const float>())
					.def(self - other<const float>())
					.def(self * other<const float>())
					.def(self / other<const float>())
					.def(tostring(self)) // uses ostream << operator
					.def_readwrite("r", &ofColor::r)
					.def_readwrite("g", &ofColor::g)
					.def_readwrite("b", &ofColor::b)
					.def_readwrite("a", &ofColor::a),
					// TODO: make the static color bindings work
					//.def_readonly("white", &ofColor::white)
					//.def_readonly("gray", &ofColor::gray)
					//.def_readonly("black", &ofColor::black)
					//.def_readonly("red", &ofColor::red)
					//.def_readonly("green", &ofColor::green)
					//.def_readonly("blue", &ofColor::blue)
					//.def_readonly("cyan", &ofColor::cyan)
					//.def_readonly("magenta", &ofColor::magenta)
					//.def_readonly("yellow", &ofColor::yellow),
					
				/// static color creators
				def("colorFromHsb", &colorFromHsb3),
				def("colorFromHsb", &ofColor::fromHsb),
				def("colorFromHex", &colorFromHex1),
				def("colorFromHex", &ofColor::fromHex),
			
				///////////////////////////////
				/// \section Pixels
				
				class_<ofPixels>("pixels")
					.def(constructor<>())
					.def(constructor<const ofPixels&>())
					.def("allocate", (void(ofPixels::*)(int,int,int)) &ofPixels::allocate)
					.def("allocateWithFormat", (void(ofPixels::*)(int,int,ofPixelFormat)) &ofPixels::allocate)
					.def("allocateWithImageType", (void(ofPixels::*)(int,int,ofImageType)) &ofPixels::allocate)
					.def("set", (void(ofPixels::*)(unsigned char)) &ofPixels::set)
					.def("set", (void(ofPixels::*)(int,unsigned char)) &ofPixels::set)
					.def("setFromPixels", &pixelsSetFromPixels)
					.def("setFromPixelsWithImageType", &pixelsSetFromPixelsWithImageType)
					.def("setFromExternalPixels", &pixelsSetFromExternalPixels)
					.def("setFromAlignedPixels", &pixelsSetFromAlignedPixels)
					.def("swap", (void(ofPixels::*)(ofPixels&)) &ofPixels::swap)
					.def("rotate90", &pixelsRotate90)
					.def("rotate90", &ofPixels::rotate90)
					.def("rotate90To", &pixelsRotate90To)
					.def("rotate90To", &ofPixels::rotate90To)
					.def("resizeTo", &pixelsResizeTo)
					.def("resizeTo", &ofPixels::resizeTo)
					.def("pasteInto", &ofPixels::pasteInto)
					.def("swapRgb", &ofPixels::swapRgb)
					.def("clear", &ofPixels::clear)
					.def("getPixelIndex", &ofPixels::getPixelIndex)
					.def("getColor", &ofPixels::getColor)
					.def("setColor", &ofPixels::setColor)
					.def("getPixel", &pixelsGetPixel)
					.def("setPixel", &pixelsSetPixel)
					.def("getChannel", &ofPixels::getChannel)
					.def("setChannel", &ofPixels::setChannel)
					.def("isAllocated", &ofPixels::isAllocated)
					.def_readonly("width", &ofPixels::getWidth)
					.def_readonly("height", &ofPixels::getHeight)
					.def_readonly("bytesPerPixel", &ofPixels::getBytesPerPixel)
					.def_readonly("bitsPerPixel", &ofPixels::getBitsPerPixel)
					.def_readonly("bytesPerChannel", &ofPixels::getBytesPerChannel)
					.def_readonly("bitsPerChannel", &ofPixels::getBitsPerChannel)
					.def_readonly("numChannels", &ofPixels::getNumChannels)
					.def_readonly("imageType", &ofPixels::getImageType)
					.def_readonly("size", &ofPixels::size)
					.enum_("format")
					[
						value("MONO", OF_PIXELS_MONO),
						value("RGB", OF_PIXELS_RGB),
						value("RGBA", OF_PIXELS_RGBA),
						value("BGRA", OF_PIXELS_BGRA),
						value("RGB565", OF_PIXELS_RGB565)
					]
					.enum_("interpolationMethod")
					[
						value("INTERPOLATE_NEAREST_NEIGHBOR", OF_INTERPOLATE_NEAREST_NEIGHBOR),
						value("INTERPOLATE_BILINEAR", OF_INTERPOLATE_BILINEAR),
						value("INTERPOLATE_BICUBIC", OF_INTERPOLATE_BICUBIC)
					],
			
				///////////////////////////////
				/// \section Texture
				
				class_<ofTexture>("texture")
					.def(constructor<>())
					.def(constructor<const ofTexture&>())
					.def("allocate", (void(ofTexture::*)(const ofPixels&)) &ofTexture::allocate)
					.def("allocate", (void(ofTexture::*)(int,int,int)) &ofTexture::allocate)
					.def("allocate", (void(ofTexture::*)(int,int,int,bool)) &ofTexture::allocate)
					.def("clear", &ofTexture::clear)
					.def("loadData", (void(ofTexture::*)(const ofPixels&)) &ofTexture::loadData)
					.def("loadScreenData", &textureLoadScreenData0)
					.def("loadScreenData", &ofTexture::loadScreenData)
					.def("setAnchorPercent", &ofTexture::setAnchorPercent)
					.def("setAnchorPoint", &ofTexture::setAnchorPoint)
					.def("resetAnchor", &ofTexture::resetAnchor)
					.def("draw", (void(ofTexture::*)(const ofPoint&,const ofPoint&,const ofPoint&,const ofPoint&)) &ofTexture::draw)
					.def("draw", (void(ofTexture::*)(float,float)) &ofTexture::draw)
					.def("draw", (void(ofTexture::*)(float,float,float)) &ofTexture::draw)
					.def("draw", (void(ofTexture::*)(float,float,float,float)) &ofTexture::draw)
					.def("drawSubsection", (void(ofTexture::*)(float,float,float,float,float,float)) &ofTexture::drawSubsection)
					.def("drawSubsection", (void(ofTexture::*)(float,float,float,float,float,float,float)) &ofTexture::drawSubsection)
					.def("drawSubsection", (void(ofTexture::*)(float,float,float,float,float,float,float,float)) &ofTexture::drawSubsection)
					.def("drawSubsection", (void(ofTexture::*)(float,float,float,float,float,float,float,float,float)) &ofTexture::drawSubsection)
					.def("readToPixels", (void(ofTexture::*)(ofPixels&)) &ofTexture::readToPixels)
					//.def("bind", &ofTexture::bind) // too low level?
					//.def("unbind", &ofTexture::unbind)
					// TODO: clean up the following ... when ofTexture is cleaned up
					.def("getCoordFromPoint", &ofTexture::getCoordFromPoint)
					.def("getCoordFromPercent", &ofTexture::getCoordFromPercent)
					.def("setWrap", &ofTexture::setTextureWrap)
					.def("setMinMagFilter", &ofTexture::setTextureMinMagFilter)
					// done TODO
					.def("isAllocated", &ofTexture::isAllocated)
					.def("getType", &textureGetType)
					.def_readonly("width",  &ofTexture::getHeight)
					.def_readonly("height", &ofTexture::getHeight)
					.property("flip", &textureGetFlip, &textureSetFlip)
					.property("tex_t", &textureGetTexT, &textureSetTexT)
					.property("tex_u", &textureGetTexU, &textureSetTexU)
					.property("tex_w", &textureGetTexW, &textureSetTexW)
					.property("tex_h", &textureGetTexH, &textureSetTexH)
					.enum_("textureType")
					[
						value("LUMINENCE", GL_LUMINANCE),
						value("RGB", GL_RGB),
						value("RGBA", GL_RGBA)
					],
			
				///////////////////////////////
				/// \section Image
				
				class_<ofImage>("image")
					.def(constructor<>())
					.def("allocate", &ofImage::allocate)
					.def("clear", &ofImage::clear)
					.def("clone", (void(ofImage::*)(const ofImage&)) &ofImage::clone)
					.def("setUseTexture", &ofImage::setUseTexture)
					.def("isUsingTexture", &ofImage::isUsingTexture)
					.def("getTexture", &ofImage::getTextureReference)
					.def("loadImage", (bool(ofImage::*)(string)) &ofImage::loadImage)
					.def("saveImage", &imageSaveImage1)
					.def("saveImage", (void(ofImage::*)(string,ofImageQualityType)) &ofImage::saveImage)
					.def("getPixels", &ofImage::getPixelsRef)
					.def("setImageType", &ofImage::setImageType)
					.def("getColor", &ofImage::getColor)
					.def("setColor", &ofImage::setColor)
					.def("setFromPixels", (void(ofImage::*)(const ofPixels&)) &ofImage::setFromPixels)
					.def("setImageType", &ofImage::setImageType)
					.def("resize", &ofImage::resize)
					.def("grabScreen", &ofImage::grabScreen)
					.def("crop", &ofImage::crop)
					.def("cropFrom", &ofImage::cropFrom)
					.def("rotate90", &imageRotate90)
					.def("rotate90", &ofImage::rotate90)
					.def("mirror", &ofImage::mirror)
					.def("setAnchorPercent", &ofImage::setAnchorPercent)
					.def("setAnchorPoint", &ofImage::setAnchorPoint)
					.def("resetAnchor", &ofImage::resetAnchor)
					.def("update", &ofImage::update)
					.def("draw", (void(ofImage::*)(float,float)) &ofImage::draw)
					.def("draw", (void(ofImage::*)(float,float,float)) &ofImage::draw)
					.def("draw", (void(ofImage::*)(float,float,float,float)) &ofImage::draw)
					.def("drawSubsection", (void(ofImage::*)(float,float,float,float,float,float)) &ofImage::drawSubsection)
					.def("drawSubsection", (void(ofImage::*)(float,float,float,float,float,float,float)) &ofImage::drawSubsection)
					.def("drawSubsection", (void(ofImage::*)(float,float,float,float,float,float,float,float)) &ofImage::drawSubsection)
					.def("drawSubsection", (void(ofImage::*)(float,float,float,float,float,float,float,float,float)) &ofImage::drawSubsection)
					.def("isAllocated", &ofImage::isAllocated)
					.def("getType", &imageGetType)
					.def_readonly("width",  &ofImage::getHeight)
					.def_readonly("height", &ofImage::getHeight)
					.enum_("imageType")
					[
						value("GRAYSCALE", OF_IMAGE_GRAYSCALE),
						value("COLOR", OF_IMAGE_COLOR),
						value("COLOR_ALPHA", OF_IMAGE_COLOR_ALPHA),
						value("UNDEFINED", OF_IMAGE_UNDEFINED)
					]
					.enum_("compressionLevel")
					[
						value("QUALITY_BEST", OF_IMAGE_QUALITY_BEST),
						value("QUALITY_HIGH", OF_IMAGE_QUALITY_HIGH),
						value("QUALITY_MEDIUM", OF_IMAGE_QUALITY_MEDIUM),
						value("QUALITY_LOW", OF_IMAGE_QUALITY_LOW),
						value("QUALITY_WORST", OF_IMAGE_QUALITY_WORST)
					],
			
				///////////////////////////////
				/// \section Font
				
				class_<ofTrueTypeFont>("font")
					.def(constructor<>())
					.def("loadFont", &fontLoadFont2)
					.def("loadFont", &fontLoadFont4)
					.def("loadFont", &fontLoadFont5)
					.def("loadFont", &fontLoadFont6)
					.def("loadFont", (void(ofTrueTypeFont::*)(string,int,bool,bool,bool,float,int)) &ofTrueTypeFont::loadFont)
					.def("isLoaded", &ofTrueTypeFont::isLoaded)
					.def("isAntiAliased", &ofTrueTypeFont::isAntiAliased)
					.def("hasFullCharSet", &ofTrueTypeFont::hasFullCharacterSet)
					.def("stringWidth", &ofTrueTypeFont::stringWidth)
					.def("stringHeight", &ofTrueTypeFont::stringHeight)
					.def("getBoundingBox", &ofTrueTypeFont::getStringBoundingBox)
					.def("draw", &ofTrueTypeFont::drawString)
					.def("drawAsShapes", &ofTrueTypeFont::drawStringAsShapes)
					.def("getCharAsPath", &ofTrueTypeFont::getCharacterAsPoints)
					//.def("getStringAsPaths", &ofTrueTypeFont::getStringAsPoints) // TODO: returns vector
					//.def("bind", &ofTrueTypeFont::bind) // TODO: too low level?
					//.def("unbind", &ofTrueTypeFont::unbind)
					.def_readonly("size", &ofTrueTypeFont::getSize)
					.def_readonly("numCharacters", &ofTrueTypeFont::getNumCharacters)
					.property("lineHeight", &ofTrueTypeFont::getLineHeight, &ofTrueTypeFont::setLineHeight)
					.property("letterSpacing", &ofTrueTypeFont::getLetterSpacing, &ofTrueTypeFont::setLetterSpacing)
					.property("spaceSize", &ofTrueTypeFont::getSpaceSize, &ofTrueTypeFont::setSpaceSize),
			
				def("setFontDpi", &ofTrueTypeFont::setGlobalDpi),
				
				///////////////////////////////
				/// \section Light
				
				class_<ofLight>("light")
					.def(constructor<>())
					.def(constructor<const ofLight&>())
					.def("clear", &ofLight::destroy)
					.def("setup", &ofLight::setup)
					.def("enable", &ofLight::enable)
					.def("disable", &ofLight::disable)
					.def("isEnabled", &ofLight::getIsEnabled)
					.def("setDirectional", &ofLight::setDirectional)
					.def("isDirectional", &ofLight::setDirectional)
					.def("setSpot", &lightSetSpotlight0)
					.def("setSpot", &lightSetSpotlight1)
					.def("setSpot", &ofLight::setSpotlight)
					.def("isSpot", &ofLight::getIsSpotlight)
					.def("getSpotCutoff", &ofLight::getSpotlightCutOff)
					.def("setSpotCutoff", &ofLight::setSpotlightCutOff)
					.def("getSpotConcentration", &ofLight::getSpotConcentration)
					.def("setSpotConcentration", &ofLight::setSpotConcentration)
					.def("setPoint", &ofLight::setPointLight)
					.def("isPoint", &ofLight::getIsPointLight)
					.def("setAttenuation", &lightSetAttenuation0)
					.def("setAttenuation", &ofLight::setAttenuation)
					// TODO: are these related to the point light only? if so, rename to getPointConstant, etc
					.def("getAttenuationConstant", &ofLight::getAttenuationConstant)
					.def("getAttenuationLinear", &ofLight::getAttenuationLinear)
					.def("getAttenuationQuadratic", &ofLight::getAttenuationQuadratic)
					// end TODO
					.def("getType", &ofLight::getType)
					.def("getAmbientColor", (ofColor(ofLight::*)(void)) &ofLight::getAmbientColor)
					.def("setAmbientColor", (void(ofLight::*)(ofColor&)) &ofLight::setAmbientColor)
					.def("setAmbientColor", &lightSetAmbientColor3)
					.def("setAmbientColor", &lightSetAmbientColor4)
					.def("setAmbientColor", &lightSetAmbientHexColor)
					.def("getDiffuseColor", (ofColor(ofLight::*)(void)) &ofLight::getDiffuseColor)
					.def("setDiffuseColor", (void(ofLight::*)(ofColor&)) &ofLight::setDiffuseColor)
					.def("setDiffuseColor", &lightSetDiffuseColor3)
					.def("setDiffuseColor", &lightSetDiffuseColor4)
					.def("setDiffuseColor", &lightSetDiffuseHexColor)
					.def("getSpecularColor", (ofColor(ofLight::*)(void)) &ofLight::getSpecularColor)
					.def("setSpecularColor", (void(ofLight::*)(ofColor&)) &ofLight::setSpecularColor)
					.def("setSpecularColor", &lightSetSpecularColor3)
					.def("setSpecularColor", &lightSetSpecularColor4)
					.def("setSpecularColor", &lightSetSpecularHexColor)
					.enum_("lightMode")
					[
						value("POINT", OF_LIGHT_POINT),
						value("SPOT", OF_LIGHT_SPOT),
						value("DIRECTIONAL", OF_LIGHT_DIRECTIONAL)
					],
					
				def("enableLighting", &ofEnableLighting),
				def("disableLighting", &ofDisableLighting),
				def("isLightingEnabled", &ofGetLightingEnabled),
				def("enableSeparateSpecularLight", &ofEnableSeparateSpecularLight),
				def("disableSeparateSpecularLight", &ofDisableSeparateSpecularLight),
				def("smoothLighting", &graphicsSmoothLighting),
				def("noSmoothLighting", &graphicsNoSmoothLighting),
				def("setGlobalAmbientColor", &ofSetGlobalAmbientColor),
				def("setGlobalAmbientColor", &graphicsSetGlobalAmbientColor3),
				def("setGlobalAmbientColor", &graphicsSetGlobalAmbientColor4),
				def("setGlobalAmbientHexColor", &graphicsSetGlobalAmbientHexColor)
			
			]; // graphics
			
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
				def("map", (float(*)(float,float,float,float,float)) &mathMap5),
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
				
				class_<ofPoint>("point")
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
					
			]; // math
			
			module(lua, "app")
			[
				///////////////////////////////
				/// \section App
				
				/// mouse
				def("mouseX", &appGetMouseX),
				def("mouseY", &appGetMouseY),
				def("pmouseX", &ofGetPreviousMouseX),
				def("pmouseY", &ofGetPreviousMouseY),
				def("mouseButton", &appGetMouseButton),
				def("mousePressed", &appGetMousePressed),
				class_<MouseVars>("mouse")
					.enum_("button")
					[
						value("LEFT", 0),
						value("MIDDLE", 1),
						value("RIGHT", 2)
					],
					
				/// keyboard
				def("keyCode", &appGetKey),
				def("keyPressed", &appGetKeyPressed),
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
				def("width", &ofGetWidth),
				def("height", &ofGetHeight),
				// TODO: are these needed by scenes? I'm thinking not
				//def("windowWidth", &ofGetWindowWidth),
				//def("windowHeight", &ofGetWindowHeight),
				//def("windowX", &ofGetWindowPositionX),
				//def("windowY", &ofGetWindowPositionY),
				//def("screenWidth", &ofGetScreenWidth),
				//def("screenHeight", &ofGetScreenHeight),
				//def("windowMode", &ofGetWindowMode), // needed? TODO: needs enums
				
				def("setWindowTitle", &ofSetWindowTitle)
				// TODO: are these needed by scenes? I'm thinking not
				//def("setWindowPosition", &ofSetWindowPosition),
				//def("setWindowSize", &ofSetWindowShape),
				//def("enableSetupScreen", &ofEnableSetupScreen),
				//def("disableSetupScreen", &ofDisableSetupScreen),
				//def("setFullscreen", &ofSetFullscreen), // needed?
				//def("toggleFullscreen", &ofToggleFullscreen), // needed?
				
				/// sync
				//def("setVerticalSync", &ofSetVerticalSync) // on by default
			
			]; // app
			
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
				
				class_<ofxTimer>("timer")
					.def(constructor<>())
					.def(constructor<unsigned int>())
					.def("set", &ofxTimer::set)
					.def("setAlarm", &ofxTimer::setAlarm)
					.def("resetAlarm", &ofxTimer::resetAlarm)
					.def("alarm", &ofxTimer::alarm)
					.def("getDiff", &ofxTimer::getDiff)
					.def("getDiffN", &ofxTimer::set)
			
			]; // util
			
			module(lua, "audio")
			[
			
				///////////////////////////////
				/// \section Sound
				class_<ofSoundPlayer>("sound")
					.def(constructor<>())
					.def("loadSound", &ofSoundPlayer::loadSound)
					.def("clear", &ofSoundPlayer::unloadSound)
					.def("isLoaded", &ofSoundPlayer::isLoaded)
					.def("play", &ofSoundPlayer::play)
					.def("pause", &soundPause)
					.def("unpause", &soundUnpause)
					.def("stop", &ofSoundPlayer::stop)
					.def("isPlaying", &ofSoundPlayer::getIsPlaying)
					.def("loop", &soundLoop)
					.def("noLoop", &soundNoLoop)
					//.def("multiPlay", &soundMultiPlay) // only works for FMOD
					//.def("noMultiPlay", &soundNoMultiPlay)
					.property("volume", &ofSoundPlayer::getVolume, &ofSoundPlayer::setVolume)
					.property("pan", &ofSoundPlayer::getPan, &ofSoundPlayer::setPan)
					.property("speed", &ofSoundPlayer::getSpeed, &ofSoundPlayer::setSpeed)
					.property("pos", &ofSoundPlayer::getPosition, &ofSoundPlayer::setPosition)
					.property("posMillis", &ofSoundPlayer::getPositionMS, &ofSoundPlayer::setPositionMS),
				
				///////////////////////////////
				/// \section Pure Data
				
				// TODO: add patch handling bindings
				//"openPatch"
				//"closePatch"
				//"clearPatches"
				
				///////////////////////////////
				/// \section Patch
				class_<pd::Patch>("patch")
					.def(constructor<>())
					.def(constructor<const string&,const string&>())
					.def("clear", &pd::Patch::clear)
					.def("isValid", &pd::Patch::isValid)
					.def(tostring(self)) // uses ostream << operator
					.def_readonly("dollarZero", &pd::Patch::dollarZero)
					.def_readonly("dollarZeroStr", &pd::Patch::dollarZeroStr)
					.def_readonly("filename", &pd::Patch::filename)
					.def_readonly("path", &pd::Patch::path)
				
			]; // audio
			
			module(lua, "osc")
			[				
				///////////////////////////////
				/// \section Osc
				
				def("send", &oscSend),
				def("sendFloat", &oscSendFloat),
				def("sendInt", &oscSendInt),
				def("sendString", &oscSendString),
				def("sendMessage", &oscSendMessage),
				def("sendBundle", &oscSendBundle),
					
				///////////////////////////////
				/// \section Message
				
				class_<ofxOscMessage>("message")
					.def(constructor<>())
					.def("clear", &ofxOscMessage::clear)
					.def("setAddress", &ofxOscMessage::setAddress)
					.def("getAddress", &ofxOscMessage::getAddress)
					.def("getRemoteIp", &ofxOscMessage::getRemoteIp)
					.def("getRemotePort", &ofxOscMessage::getRemotePort)
					.def("setRemote", &ofxOscMessage::setRemoteEndpoint)
					.def("getType", &ofxOscMessage::getArgType)
					.def("getTypeName", &ofxOscMessage::getArgTypeName)
					.def("getInt", &ofxOscMessage::getArgAsInt32)
					.def("getFloat", &ofxOscMessage::getArgAsFloat)
					.def("getString", &ofxOscMessage::getArgAsString)
					.def("addInt", &ofxOscMessage::addIntArg)
					.def("addFloat", &ofxOscMessage::addFloatArg)
					.def("addString", &ofxOscMessage::addStringArg)
					.def_readonly("numArgs", &ofxOscMessage::getNumArgs)
					.enum_("argType")
					[
						value("NONE", OFXOSC_TYPE_NONE),
						value("INT", OFXOSC_TYPE_INT32),
						value("FLOAT", OFXOSC_TYPE_FLOAT),
						value("STRING", OFXOSC_TYPE_STRING),
						value("BLOB", OFXOSC_TYPE_BLOB),
						value("BUNDLE", OFXOSC_TYPE_BUNDLE),
						value("OUTOFBOUNDS", OFXOSC_TYPE_INDEXOUTOFBOUNDS)
					],
					
				///////////////////////////////
				/// \section Bundle
				
				class_<ofxOscBundle>("bundle")
					.def(constructor<>())
					.def("clear", &ofxOscBundle::clear)
					.def("addMessage", &ofxOscBundle::addMessage)
					.def("addBundle", &ofxOscBundle::addBundle)
					.def("getMessage", &ofxOscBundle::getMessageAt)
					.def("getBundle", &ofxOscBundle::getBundleAt)
					.def_readonly("numMessages", &ofxOscBundle::getMessageCount)
					.def_readonly("numBundles", &ofxOscBundle::getBundleCount)
			
			]; // osc
			
			// TODO: add ofxUI bindings
			//module(lua, "gui")
			//[
				///////////////////////////////
				/// \section Gui
				
				//class_<
				
			//]; // gui
			
			/// rc-specifics
			module(lua, "rc")
			[
				/// to override lua print and write functions
				def("print", &print),
				def("write", &write)
			
			]; // rc
		}
		
		/// \section Function & Object Wrappers
		
		/// background
		static void graphicsBackground(int brightness) {ofBackground(brightness);}
		static void graphicsBackground(int r, int g, int b) {ofBackground(r, g, b);}
		static void graphicsBackgroundHex(int hexColor) {ofBackgroundHex(hexColor);}
		static void graphicsClear(float brightness) {ofClear(brightness);}
		static void graphicsAutoBackground() {ofSetBackgroundAuto(true);}
		static void graphicsNoAutoBackground() {ofSetBackgroundAuto(false);}
		
		/// primitives
		static void graphicsCurve(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, const ofPoint& p4)
			{ofCurve(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z, p4.x, p4.y, p4.z);}
		static void graphicsBezier(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, const ofPoint& p4)
			{ofBezier(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z, p4.x, p4.y, p4.z);}
		
		/// polygons
		static void graphicsEndShape() {ofEndShape();}
		static void graphicsNextContour() {ofNextContour();}
		
		/// 3d utils
		static void graphicsDrawGrid0() {
			ofDrawGrid();
		}
		static void graphicsDrawGrid3(float scale, float ticks, bool labels) {
			ofDrawGrid(scale, ticks, labels);
		}
		static void graphicsDrawGridPlane1(float scale) {
			ofDrawGridPlane(scale);
		}
		static void graphicsDrawArrow2(const ofPoint& start, const ofPoint& end) {
			ofDrawArrow(start, end);
		}

		/// polyline
		static void polylineAddVertex2(ofPolyline* line, float x, float y) {
			line->addVertex(x, y);
		}
		static void polylineLineTo2(ofPolyline* line, float x, float y) {
			line->lineTo(x, y);
		}
		static void polylineArcPoint5(ofPolyline* line, const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd) {
			line->arc(center, radiusX, radiusY, angleBegin, angleEnd);
		}
		static void polylineArcPoint6(ofPolyline* line, const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise) {
			line->arc(center, radiusX, radiusY, angleBegin, angleEnd, clockwise);
		}
		static void polylineArcFloat6(ofPolyline* line, float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd) {
			line->arc(x, y, radiusX, radiusY, angleBegin, angleEnd);
		}
		static void polylineArcFloat7(ofPolyline* line, float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd) {
			line->arc(x, y, z, radiusX, radiusY, angleBegin, angleEnd);
		}
		static void polylineArcNegativePoint5(ofPolyline* line, const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd) {
			line->arcNegative(center, radiusX, radiusY, angleBegin, angleEnd);
		}
		static void polylineArcNegativeFloat6(ofPolyline* line, float x, float y, float radiusX, float radiusY, float angleBegin, float angleEnd) {
			line->arcNegative(x, y, radiusX, radiusY, angleBegin, angleEnd);
		}
		static void polylineArcNegativeFloat7(ofPolyline* line, float x, float y, float z, float radiusX, float radiusY, float angleBegin, float angleEnd) {
			line->arcNegative(x, y, z, radiusX, radiusY, angleBegin, angleEnd);
		}
		static void polyLineCurveToPoint1(ofPolyline* line, const ofPoint& to) {line->curveTo(to);}
		static void polyLineCurveToFloat2(ofPolyline* line, float x, float y) {line->curveTo(x, y);}
		static void polyLineCurveToFloat3(ofPolyline* line, float x, float y, float z) {line->curveTo(x, y, z);}
		
		static void polylineBezierToPoint3(ofPolyline* line, const ofPoint & cp1, const ofPoint & cp2, const ofPoint & to) {
			line->bezierTo(cp1, cp2, to);
		}
		static void polylineBezierToFloat6(ofPolyline* line, float cx1, float cy1, float cx2, float cy2, float x, float y) {
			line->bezierTo(cx1, cy1, cx2, cy2, x, y);
		}
		static void polylineBezierToFloat9(ofPolyline* line, float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z) {
			line->bezierTo(cx1, cy1, cz1, cx2, cy2, cz2, x, y, z);
		}
		static void polylineQuadBezierToPoint3(ofPolyline* line, const ofPoint & cp1, const ofPoint & cp2, const ofPoint & to) {
			line->quadBezierTo(cp1, cp2, to);
		}
		static void polylineQuadBezierToFloat6(ofPolyline* line, float cx1, float cy1, float cx2, float cy2, float x, float y) {
			line->quadBezierTo(cx1, cy1, cx2, cy2, x, y);
		}
		static void polylineQuadBezierToFloat9(ofPolyline* line, float cx1, float cy1, float cz1, float cx2, float cy2, float cz2, float x, float y, float z) {
			line->quadBezierTo(cx1, cy1, cz1, cx2, cy2, cz2, x, y, z);
		}
		static ofPolyline polylineGetSmoothed1(ofPolyline* line, float smoothingSize) {
			return line->getSmoothed(smoothingSize);
		}
		static ofPoint polylineGetClosestPoint1(ofPolyline* line, const ofPoint& target) {
			return line->getClosestPoint(target);
		}
		static unsigned int polylineGetClosestPointIndex(ofPolyline* line, const ofPoint& target) {
			unsigned int index = 0;
			line->getClosestPoint(target, &index);
			return index;
		}
		static void polylineSimplify0(ofPolyline* line) {
			line->simplify();
		}
		static ofPoint polylineGetPoint(ofPolyline* line, unsigned int index) {
			return line->getVertices()[index];
		}
		static void polylineSetPoint(ofPolyline* line, unsigned int index, const ofPoint& p) {
			line->getVertices()[index] = p;
		}
				
		/// path
		static void pathMoveTo1(ofPath* path, const ofPoint& p) {
			path->moveTo(p);
		}
		static void pathMoveTo2(ofPath* path, float x, float y) {
			path->moveTo(x, y);
		}
		static void pathCurveTo1(ofPath* path, const ofPoint& p) {
			path->curveTo(p);
		}
		static ofPolyline pathGetOutline(ofPath* path, unsigned int index) {
			return path->getOutline()[index];
		}
		// Todo: ofPolyline dosen't have a copy operator yet ...
//		static void pathSetOutline(ofPath* path, unsigned int index, const ofPolyLine& line) {
//			path->getOutline()[index] = line;
//		}
		static int pathNumOutlines(ofPath* path) {
			return path->getOutline().size();
		}
		static void pathEnableShapeColor(ofPath* path) {
			path->setUseShapeColor(true);
		}
		static void pathDisableShapeColor(ofPath* path) {
			path->setUseShapeColor(false);
		}
		static void pathSimplify0(ofPath* path) {
			path->simplify();
		}
		static void pathTranslate2(ofPath* path, float x, float y) {
			path->translate(ofPoint(x, y));
		}
		static void pathTranslate3(ofPath* path, float x, float y, float z) {
			path->translate(ofPoint(x, y, z));
		}
		static void pathRotate1(ofPath* path, float angle) {
			path->rotate(angle, ofPoint(0, 0, 1));
		}
		static void pathRotateX(ofPath* path, float angle) {
			path->rotate(angle, ofPoint(1, 0, 0));
		}
		static void pathRotateY(ofPath* path, float angle) {
			path->rotate(angle, ofPoint(0, 1, 0));
		}
		static void pathRotateZ(ofPath* path, float angle) {
			path->rotate(angle, ofPoint(0, 0, 1));
		}
		static void pathRotate4(ofPath* path, float angle, float vecX, float vecY, float vecZ) {
			path->rotate(angle, ofPoint(vecX, vecY, vecZ));
		}
		
		/// mesh
		static ofPoint meshGetTexCoord(ofMesh* mesh, int i) {
			return ofPoint(mesh->getTexCoord(i));
		}
		static void meshSetTexCoord(ofMesh* mesh, int i, const ofPoint& p) {
			mesh->setTexCoord(i, ofVec2f(p.x, p.y));
		}
		static void meshAddTexCoord(ofMesh* mesh, const ofPoint& p) {
			mesh->addTexCoord(ofVec2f(p.x, p.y));
		}
		static void meshSave1(ofMesh* mesh, string path) {
			mesh->save(path);
		}

		/// color
		static void colorSet1(ofColor* color, float gray) {
			color->set(gray);
		}
		static void colorSet3(ofColor* color, float r, float g, float b) {
			color->set(r, g, b);
		}
		static void colorSetHsb3(ofColor* color, float hue, float saturation, float brightness) {
			color->setHsb(hue, saturation, brightness);
		}
		static ofColor colorFromHsb3(float hue, float saturation, float brightness) {
			return ofColor::fromHsb(hue, saturation, brightness);
		}
		static ofColor colorFromHex1(int hexColor) {
			return ofColor::fromHex(hexColor);
		}

		/// pixels
		static void pixelsSetFromPixels(ofPixels* pixels, const ofPixels& newPixels, int w, int h, int channels) {
			pixels->setFromPixels(newPixels.getPixels(), w, h, channels);
		}
		static void pixelsSetFromPixelsWithImageType(ofPixels* pixels, const ofPixels& newPixels, int w, int h, ofImageType type) {
			pixels->setFromPixels(newPixels.getPixels(), w, h, type);
		}
		static void pixelsSetFromExternalPixels(ofPixels* pixels, ofPixels& newPixels, int w, int h, int channels) {
			pixels->setFromExternalPixels(newPixels.getPixels(), w, h, channels);
		}
		static void pixelsSetFromAlignedPixels(ofPixels* pixels, ofPixels& newPixels, int w, int h, int channels, int stride) {
			pixels->setFromAlignedPixels(newPixels.getPixels(), w, h, channels, stride);
		}
		static unsigned char pixelsGetPixel(ofPixels* pixels, unsigned int index) {
			return pixels->getPixels()[index];
		}
		static void pixelsSetPixel(ofPixels* pixels, unsigned int index, unsigned char p) {
			pixels->getPixels()[index] = p;
		}
		static void pixelsRotate90(ofPixels* pixels) {
			pixels->rotate90(1);
		}
		static void pixelsRotate90To(ofPixels* pixels, ofPixels& dst) {
			pixels->rotate90To(dst, 1);
		}
		static bool pixelsResizeTo(ofPixels* pixels, ofPixels& dst) {
			return pixels->resizeTo(dst);
		}
		
		/// texture
		static void textureLoadScreenData0(ofTexture* texture) {
			texture->loadScreenData(0, 0, ofGetWidth(), ofGetHeight());
		}
		static int textureGetType(ofTexture* texture) {
			return texture->getTextureData().glTypeInternal;
		}
		static void textureSetFlip(ofTexture* texture, bool flip) {
			texture->getTextureData().bFlipTexture = flip;
		}
		static bool textureGetFlip(ofTexture* texture) {
			return texture->getTextureData().bFlipTexture;
		}
		static float textureGetTexT(ofTexture* texture) {
			return texture->getTextureData().tex_t;
		}
		static void textureSetTexT(ofTexture* texture, float tex_t) {
			texture->getTextureData().tex_t = tex_t;
		}
		static float textureGetTexU(ofTexture* texture) {
			return texture->getTextureData().tex_u;
		}
		static void textureSetTexU(ofTexture* texture, float tex_u) {
			texture->getTextureData().tex_u = tex_u;
		}
		static float textureGetTexW(ofTexture* texture) {
			return texture->getTextureData().tex_w;
		}
		static void textureSetTexW(ofTexture* texture, float tex_w) {
			texture->getTextureData().tex_t = tex_w;
		}
		static float textureGetTexH(ofTexture* texture) {
			return texture->getTextureData().tex_h;
		}
		static void textureSetTexH(ofTexture* texture, float tex_h) {
			texture->getTextureData().tex_t = tex_h;
		}
		
		/// image
		static void imageSaveImage1(ofImage* image, string fileName) {
			image->saveImage(fileName);
		}
		static void imageRotate90(ofImage* image) {
			image->rotate90(1);
		}
		static ofImageType imageGetType(ofImage* image) {
			return image->getPixelsRef().getImageType();
		}

		/// global mouse vars
		static float appGetMouseX();
		static float appGetMouseY();
		static int appGetMouseButton();
		static bool appGetMousePressed();
		static int appGetKey();
		static bool appGetKeyPressed();
		
		/// font
		static void fontLoadFont2(ofTrueTypeFont* font, string filename, int fontsize) {
			font->loadFont(filename, fontsize);
		}
		static void fontLoadFont4(ofTrueTypeFont* font, string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet) {
			font->loadFont(filename, fontsize, _bAntiAliased, _bFullCharacterSet);
		}
		static void fontLoadFont5(ofTrueTypeFont* font, string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours) {
			font->loadFont(filename, fontsize, _bAntiAliased, _bFullCharacterSet, makeContours);
		}
		static void fontLoadFont6(ofTrueTypeFont* font, string filename, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours, float simplifyAmt) {
			font->loadFont(filename, fontsize, _bAntiAliased, _bFullCharacterSet, simplifyAmt);
		}
		
		/// light
		static void lightSetSpotlight0(ofLight* light) {
			light->setSpotlight();
		}
		static void lightSetSpotlight1(ofLight* light, float spotCutoff) {
			light->setSpotlight(spotCutoff);
		}
		static void lightSetAttenuation0(ofLight* light) {
			light->setAttenuation();
		}
		static void lightSetAmbientColor3(ofLight* light, int r, int g, int b) {
			light->setAmbientColor(ofColor(r, g, b));
		}
		static void lightSetAmbientColor4(ofLight* light, int r, int g, int b, int a) {
			light->setAmbientColor(ofColor(r, g, b, a));
		}
		static void lightSetAmbientHexColor(ofLight* light, int color) {
			light->setAmbientColor(ofColor(color));
		}
		static void lightSetDiffuseColor3(ofLight* light, int r, int g, int b) {
			light->setDiffuseColor(ofColor(r, g, b));
		}
		static void lightSetDiffuseColor4(ofLight* light, int r, int g, int b, int a) {
			light->setDiffuseColor(ofColor(r, g, b, a));
		}
		static void lightSetDiffuseHexColor(ofLight* light, int color) {
			light->setDiffuseColor(ofColor(color));
		}
		static void lightSetSpecularColor3(ofLight* light, int r, int g, int b) {
			light->setSpecularColor(ofColor(r, g, b));
		}
		static void lightSetSpecularColor4(ofLight* light, int r, int g, int b, int a) {
			light->setSpecularColor(ofColor(r, g, b, a));
		}
		static void lightSetSpecularHexColor(ofLight* light, int color) {
			light->setSpecularColor(ofColor(color));
		}
		
		/// global light
		static void graphicsSmoothLighting() {ofSetSmoothLighting(true);}
		static void graphicsNoSmoothLighting() {ofSetSmoothLighting(false);}
		static void graphicsSetGlobalAmbientColor3(int r, int g, int b) {ofSetGlobalAmbientColor(ofColor(r, g, b));}
		static void graphicsSetGlobalAmbientColor4(int r, int g, int b, int a) {ofSetGlobalAmbientColor(ofColor(r, g, b, a));}
		static void graphicsSetGlobalAmbientHexColor(int color) {ofSetGlobalAmbientColor(ofColor(color));}
		
		/// math
		static float mathMap5(float value, float inputMin, float inputMax, float outputMin, float outputMax) {
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
		
		/// audio
		static void soundPause(ofSoundPlayer* sound) {sound->setPaused(true);}
		static void soundUnpause(ofSoundPlayer* sound) {sound->setPaused(false);}
		static void soundLoop(ofSoundPlayer* sound) {sound->setLoop(true);}
		static void soundNoLoop(ofSoundPlayer* sound) {sound->setLoop(false);}
		
		/// osc
		static void oscSend(string address) {
			ofxOscMessage msg;
			msg.setAddress(address);
			Global::instance().osc.sendOscFromScript(msg);
		}
		static void oscSendFloat(string address, float f) {
			ofxOscMessage msg;
			msg.setAddress(address);
			msg.addFloatArg(f);
			Global::instance().osc.sendOscFromScript(msg);
		}
		static void oscSendInt(string address, int i) {
			ofxOscMessage msg;
			msg.setAddress(address);
			msg.addIntArg(i);
			Global::instance().osc.sendOscFromScript(msg);
		}
		static void oscSendString(string address, string s) {
			ofxOscMessage msg;
			msg.setAddress(address);
			msg.addStringArg(s);
			Global::instance().osc.sendOscFromScript(msg);
		}
		static void oscSendMessage(ofxOscMessage& msg) {
			Global::instance().osc.sendOscFromScript(msg);
		}
		static void oscSendBundle(ofxOscBundle& bundle) {
			Global::instance().osc.sender.sendBundle(bundle);
		}
		
		/// console io
		static void print(const std::string& message) {
			Global::instance().scriptEngine.print(message);
		}
		
		static void write(const std::string& message) {
			Global::instance().scriptEngine.write(message);
		}
};
