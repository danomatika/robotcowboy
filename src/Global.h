/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/robotcowboy for documentation
 *
 */
#pragma once

#include "ofxOsc.h"

// a singleton for global variables
class Global {

    public:

        /// singleton data access
		/// returns a reference to itself
		/// creates a new object on the first call
        static Global& instance();

        /// get a reference to the OscSender and OscReceiver
        inline ofxOscSender& getOscSender() {return oscSender;}
        //inline osc::OscReceiver& getOscReceiver() {return oscReceiver;}
		
		/// \section Variables
		
		string oscSendAddress;
		int oscSendPort;

    private:
        
        ofxOscSender oscSender;       	///< global osc sender
        ofxOscReceiver oscReceiver;		///< global osc receiver
        
        // hide all the constructors, copy functions here
        Global(); 							// singleton constructor
        Global(Global const&);    			// not defined, not copyable
        Global& operator = (Global const&);	// not defined, not assignable
};
