#! /bin/bash

WD=$(dirname $0)

VER=0.42.5

###

cd $WD

## get latest source
svn export https://pure-data.svn.sourceforge.net/svnroot/pure-data/tags/pd-extended/$VER/externals/ externals

## copy external abs and source

# iemlib
mkdir -p ../src/externals/iemlib
#mkdir -p ../bin/data/externals/iemlib
#rm -v externals/iemlib/iemlib2/src/wrap.c
#cp -Rv externals/iemlib/iemlib2/*.pd ../bin/data/externals/iemlib
cp -Rv externals/iemlib/iemlib2/src/*.h ../src/externals/iemlib
cp -Rv externals/iemlib/iemlib2/src/*.c ../src/externals/iemlib

# mrpeach
mkdir -p ../src/externals/mrpeach
#mkdir -p ../bin/data/externals/mrpeach
#cp -Rv externals/mrpeach/osc/*.pd ../bin/data/externals/mrpeach
cp -Rv externals/mrpeach/osc/*.h ../src/externals/mrpeach
cp -Rv externals/mrpeach/osc/*.c ../src/externals/mrpeach
#cp -Rv externals/mrpeach/net/*.pd ../bin/data/externals/mrpeach
cp -Rv externals/mrpeach/net/*.h ../src/externals/mrpeach
cp -Rv externals/mrpeach/net/*.c ../src/externals/mrpeach
cp -Rv externals/mrpeach/midifile/*.c ../src/externals/mrpeach

## cleanup
rm -rfv externals
