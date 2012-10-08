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
#include "WebServer.h"
#include "ofLog.h"

#import "DDLog.h"
#import "DDTTYLogger.h"

static const int ddLogLevel = LOG_LEVEL_ERROR;

HTTPServer* WebServer::server = nil;

//--------------------------------------------------------------
void WebServer::start(std::string webFolder) {

	// configure logging system
	[DDLog addLogger:[DDTTYLogger sharedInstance]];

	// create DAV server
	server = (HTTPServer*) [[HTTPServer alloc] init];
	[server setConnectionClass:[DAVConnection class]];
	[server setPort:8080];

	// enable Bonjour
	[server setType:@"_http._tcp."];

	// set document root
	[server setDocumentRoot:[[NSString stringWithUTF8String:webFolder.c_str()] stringByExpandingTildeInPath]];
	ofLogVerbose() << "WebServer: set root to " << webFolder;

	// start DAV server
	NSError* error = nil;
	if(![server start:&error]) {
		//DDLogError(@"Error starting HTTP Server: %@", error);
		ofLogError() << "WebServer: error starting server: " << [[error localizedDescription] UTF8String];
	}
	ofLogVerbose() << "WebServer: started server, port: " << [server port];
}

//--------------------------------------------------------------
void WebServer::stop() {
	[server stop];
	server = nil;
}

//--------------------------------------------------------------
bool WebServer::isRunning() {
	if([server isRunning])
		return true;
	return false;
}