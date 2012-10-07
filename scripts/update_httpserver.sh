#! /bin/bash

WD=$(dirname $0)

###

cd $WD

# get latest source
git clone git://github.com/robbiehanson/CocoaHTTPServer.git

# make folder
mkdir -p ../src/ios/CocoaHTTPServer

# copy rj sources
cp -Rv CocoaHTTPServer/Core ../src/ios/CocoaHTTPServer
cp -Rv CocoaHTTPServer/Extensions ../src/ios/CocoaHTTPServer
cp -Rv CocoaHTTPServer/Vendor ../src/ios/CocoaHTTPServer
cp -Rv CocoaHTTPServer/LICENSE.txt ../src/ios/CocoaHTTPServer

# cleanup
rm -rfv CocoaHTTPServer

