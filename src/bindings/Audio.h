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

#include "ofxPd.h"

namespace lua {

class Audio {

	public:
	
		static void bind(ofxLua& lua) {
		
			using namespace luabind;

			module(lua, "audio")
			[
			
				///////////////////////////////
				/// \section Sound
				class_<ofSoundPlayer>("Sound")
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
				class_<pd::Patch>("Patch")
					.def(constructor<>())
					.def(constructor<const string&,const string&>())
					.def("clear", &pd::Patch::clear)
					.def("isValid", &pd::Patch::isValid)
					.def(tostring(self)) // uses ostream << operator
					.def_readonly("dollarZero", &pd::Patch::dollarZero)
					.def_readonly("dollarZeroStr", &pd::Patch::dollarZeroStr)
					.def_readonly("filename", &pd::Patch::filename)
					.def_readonly("path", &pd::Patch::path)
			];
		}
		
		/// \section Function & Object Wrappers
		
		/// audio
		static void soundPause(ofSoundPlayer* sound) {sound->setPaused(true);}
		static void soundUnpause(ofSoundPlayer* sound) {sound->setPaused(false);}
		static void soundLoop(ofSoundPlayer* sound) {sound->setLoop(true);}
		static void soundNoLoop(ofSoundPlayer* sound) {sound->setLoop(false);}
};

} // namespace
