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

extern "C" {

	void iemlib2_setup();
	
	// mrpeach
	void midifile_setup();
	void packOSC_setup();
	void pipelist_setup();
	void routeOSC_setup();
	void unpackOSC_setup();
}

class Externals {

	public:

		static void setup() {
		
			iemlib2_setup();
			
			// mrpeach
			midifile_setup();
			packOSC_setup();
			pipelist_setup();
			routeOSC_setup();
			unpackOSC_setup();
		}
};
