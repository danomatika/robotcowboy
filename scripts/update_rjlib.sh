#! /bin/bash

WD=$(dirname $0)

###

cd $WD

# get latest source
git clone git://github.com/rjdj/rjlib.git rjlib

# copy rj sources
cp -Rv rjlib/rj ../bin/data/externals

# cleanup
rm -rfv rjlib

