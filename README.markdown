robotcowboy app: the software framework for the robotcowboy project
===================================

Copyright (c) [Dan Wilcox](danomatika.com) 2007, 2011

This is a very, very minimal readme on the legacy (2007-2011) robotcowboy framework. The new robotcowboy 2.0 app framework will be forthcoming soon ...

[robotcowboy](http://robotcowboy.com)

DESCRIPTION
-----------

This is the legacy script and Pure Data framework for [robotcowboy](http://robotcowboy.com), a Linux-based wearable computer musical performance system. The Pure Data patch libraries and run scripts are packaged into .deb Debian package files (using checkinstall) to be deployed on an Ubuntu system.

2 external programs can also be packaged:

* rc-visual, a simple SDL graphics engine: https://github.com/danomatika/rc-visual
* rc-unitd, a hid device daemon: https://github.com/danomatika/rc-unitd

USAGE
-----

The following scripts are provided to automate the build process:

* clone_externals: git clones rc-visual and rc-unitd
* build_packages: builds rc-visual and rc-unitd, builds debian packages using checkinstall
* clean_all: cleans sources and built packages

Note, you will need:

* checkinstall, a Linux application used to build the Debian packages
* the individual libs required to build rc-visual & rc-unitd (see their readmes) 

