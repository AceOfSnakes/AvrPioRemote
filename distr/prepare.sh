#!/bin/bash -e

DIR=$PWD
cd ..
LEVELUP=
if [ "x"$1 != "x" ] ; then
  cd ..
  LEVELUP=/..
fi

NAME=$(basename "$PWD")
cd $DIR

# echo "==============================================================="
#
# $(find ../../.. -name ${NAME}.pro)
# exit 0


TARGET=/tmp/${NAME}
SOURCE=$DIR
rm -rf $TARGET
mkdir -p $TARGET/debian
cp ${SOURCE}/debian/ -r $TARGET/
cp -r ${SOURCE}${LEVELUP}/../src/ $TARGET/src/
cp  ${SOURCE}${LEVELUP}/../*.pro $TARGET/

cd $TARGET
echo "==============================================================="
echo $0 PWD: $(pwd)
echo "==============================================================="

PWDFROM=$(pwd)
