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

#include "ofxPd.h"

class ofxOscMessage;
class ofxOscBundle;

class AudioEngine {

	public:
	
		AudioEngine();
	
		bool setup(const int numOutChannels, const int numInChannels,
				   const int sampleRate, const int ticksPerBuffer);
		void clear() {}
		
		/// clears the current patch
		void clearPatch();
		
		/// load a new patch
		/// clears the current patch
		bool loadPatch(std::string patch);
		
		/// exit and reload the current patch
		bool reloadPatch();
		
		/// handle any incoming osc messages
		void update();
		
		/// send an osc message to pd
		/// sends to OSC_IN
		void sendOsc(ofxOscMessage& msg);
		void sendOsc(ofxOscBundle& bundle);
		
		ofxPd pd;
		bool sendsOscOut;
		
	private:
		
		pd::Patch currentPatch;
};
