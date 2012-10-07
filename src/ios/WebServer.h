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

#include <string>

#import "HTTPServer.h"
#import "DAVConnection.h"

// webdav server
class WebServer {

	public:
		
		static void start(std::string webFolder);
		static void stop();
		static bool isRunning();
		
	private:
	
		//struct HTTPServer; // forward declaration for Obj-C wrapper
		static HTTPServer * server;
};
