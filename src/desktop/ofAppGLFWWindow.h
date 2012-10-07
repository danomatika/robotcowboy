#pragma once

#include "ofConstants.h"

#ifdef TARGET_LINUX
#include <GL/glfw.h>   // For GLFW, OpenGL and GLU
#elif defined(TARGET_OSX)
#include <GL/glfw.h>
#elif defined(TARGET_WIN32)
#include "GL/glfw.h"
#endif

#include "ofAppBaseWindow.h"
#include "ofEvents.h"

class ofBaseApp;

class ofAppGLFWWindow : public ofAppBaseWindow {

public:

	ofAppGLFWWindow();
	~ofAppGLFWWindow(){}

	void setupOpenGL(int w, int h, int screenMode);
	void initializeWindow();
	void runAppViaInfiniteLoop(ofBaseApp * appPtr);

	static void exitApp();

	void hideCursor();
	void showCursor();
	
	int getHeight();
	int getWidth();
	
	ofPoint		getWindowPosition();
	ofPoint		getWindowSize();
	ofPoint		getScreenSize();

	void setWindowTitle(string title);
	void setWindowPosition(int x, int y);
	void setWindowShape(int w, int h);


	int			getWindowMode();
	float		getFrameRate();
	void		setFrameRate(float targetRate);
	double		getLastFrameTime();


	int			getFrameNum();

	void		setFullscreen(bool fullscreen);
	void		toggleFullscreen();

	void		enableSetupScreen();
	void		disableSetupScreen();

	void		setVerticalSync(bool bSync);


	//GLFW specifics
	void		setFSAASamples(int samples);
	void		listVideoModes();
	bool		isWindowIconified();
	bool		isWindowActive();
	bool		isWindowResizeable();
	void		iconify(bool bIconify);

private:
	// callbacks
	void			display(void);
	void 			idle(void);

	static void 	mouse_cb(int button, int state);
	static void 	motion_cb(int x, int y);
	static void 	keyboard_cb(int key, int state);
	static void 	resize_cb(int w, int h);
	static int 		exit_cb();


	//utils
	void			changeMode();

	int				samples;

	int				nFrameCount;

	int 			millisForFrame;
	int 			prevMillis;
	int 			diffMillis;
	bool 			bFrameRateSet;
	double			lastFrameTime;
	double			fps;

	int				windowMode;
	bool			bNewScreenMode;

	bool			bEnableSetupScreen;

	float			frameRate;

	int				requestedWidth;
	int				requestedHeight;

	int 			nonFullScreenW;
	int 			nonFullScreenH;
	int 			nonFullScreenX;
	int 			nonFullScreenY;

};
