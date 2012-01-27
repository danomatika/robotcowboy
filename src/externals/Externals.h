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
