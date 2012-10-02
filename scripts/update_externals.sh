#! /bin/bash
#
# checks out and copies externals sources/patches from pd svn
#
# browse: https://pure-data.svn.sourceforge.net/svnroot/pure-data/tags/pd-extended/0.42.5/externals/
#

WD=$(dirname $0)

VER=0.42.5

SRC_DIR=../src/externals
PATCH_DIR=../bin/data/externals

###

cd $WD

## get latest source
svn export https://pure-data.svn.sourceforge.net/svnroot/pure-data/tags/pd-extended/$VER/externals/ externals

## copy external abs and source

# iemlib
mkdir -p $SRC_DIR/iemlib
#mkdir -p ../bin/data/externals/iemlib
#rm -v externals/iemlib/iemlib2/src/wrap.c # overrides pd's wrap
cp -v externals/iemlib/LICENCE.txt $SRC_DIR/iemlib
cp -v externals/iemlib/CHANGES.txt $SRC_DIR/iemlib
cp -v externals/iemlib/CONTENT.txt $SRC_DIR/iemlib
#cp -Rv externals/iemlib/iemlib2/*.pd ../bin/data/externals/iemlib
cp -Rv externals/iemlib/iemlib2/src/*.h $SRC_DIR/iemlib
cp -Rv externals/iemlib/iemlib2/src/*.c .$SRC_DIR/iemlib

# mrpeach
mkdir -p $SRC_DIR/mrpeach
#mkdir -p ../bin/data/externals/mrpeach
#cp -Rv externals/mrpeach/osc/*.pd ../bin/data/externals/mrpeach
cp -Rv externals/mrpeach/osc/*.h $SRC_DIR/mrpeach
cp -Rv externals/mrpeach/osc/*.c $SRC_DIR/mrpeach
#cp -Rv externals/mrpeach/net/*.pd ../bin/data/externals/mrpeach
cp -Rv externals/mrpeach/net/*.h $SRC_DIR/mrpeach
cp -Rv externals/mrpeach/net/*.c $SRC_DIR/mrpeach
cp -Rv externals/mrpeach/midifile/*.c $SRC_DIR/mrpeach
#cp -Rv externals/mrpeach/sqosc~/*.c $SRC_DIR/mrpeach # doesn't build on ios

# ggee
mkdir -p $SRC_DIR/ggee
cp -v externals/ggee/control/getdir.c $SRC_DIR/ggee
cp -v externals/ggee/control/stripdir.c $SRC_DIR/ggee
cp -v externals/ggee/control/moog~.c $SRC_DIR/ggee

# mapping
mkdir -p $PATCH_DIR/mapping
cp -v externals/mapping/LICENSE.txt $PATCH_DIR/mapping
cp -Rv externals/mapping/*.pd $PATCH_DIR/mapping

### GPL sources, to be replaced ...

# creb
#mkdir -p $SRC_DIR/creb
#cp -v externals/creb/modules/xfm~.c $SRC_DIR/creb

# zexy
#mkdir -p $PATCH_DIR/zexy
#mkdir -p $SRC_DIR/zexy
#cp -Rv externals/zexy/abs/* $PATCH_DIR/zexy
#cp -Rv externals/zexy/src/*.h $SRC_DIR/zexy
#cp -Rv externals/zexy/src/*.c $SRC_DIR/zexy

## cleanup
rm -rf externals
