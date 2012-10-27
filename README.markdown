robotcowboy
===========

the software framework for the [robotcowboy project](http://robotcowboy.com)

Copyright (c) [Dan Wilcox](danomatika.com) 2007, 2011-2012

See https://github.com/danomatika/robotcowboy for documentation

Description
-----------

robotcowboy is an audio/visual environment for performance. It features:

* a Pure Data audio instance
* a Lua scripting environment inspired by OpenFrameworks/Processing
* a scene system including playlists
* midi io
* inter-app OSC communication
* 2d physics using Box2d

It's directly inspired by the original RjDj app and Love2d. I decided to write my own app in order to add the features I needed for a live performance focus.

NOTE: THIS IS CURRENTLY IN ALPHA DEVELOPMENT. BETA COMING SOON.

Building
--------
robotcowboy is being developed against the current stable version of openFrameworks. You can get it from [openframeworks.cc](http://www.openframeworks.cc/download) or on [Github](https://github.com/openframeworks/openFrameworks).

Additionally, you will need the following add ons:

* ofxOsc (included with OF)
* [ofxPd](https://github.com/danomatika/ofxPd)
* [ofxLua](https://github.com/danomatika/ofxLua)
* [ofxMidi](https://github.com/chrisoshea/ofxMidi)
* [ofxAppUtils](https://github.com/danomatika/ofxAppUtils)
* [ofxUI](https://github.com/rezaali/ofxUI)
* ofxBox2d (use [my fork](https://github.com/danomatika/ofxBox2d) on Github)

They should be installed in the `addons` dir in the OpenFrameworks folder.


Running
-------
Running the app will currently run a playlist of test scenes.

### Scenes

A scene is a single program run by the environment written as a combination of Pure Data patches and Lua scripts. robotcowboy looks for \_main.pd and \_main.lua as their respective entry points and OSC is used to communicate between the audio and visual engines.

*More detailed info to come soon.*

Developing
----------

You can help develop robotcowboy on GitHub: [https://github.com/danomatika/robotcowboy](https://github.com/danomatika/robotcowboy)

Create an account, clone or fork the repo, then request a push/merge.

If you find any bugs or suggestions please log them to GitHub as well.
