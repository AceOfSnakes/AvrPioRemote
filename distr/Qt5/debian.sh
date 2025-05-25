TARGET=/tmp/xxx
SOURCE=../../../
rm -rf $TARGET
mkdir -p $TARGET/debian
cp . -r $TARGET
cp -r ../../src/ $TARGET/src/
cp  ../../* $TARGET/
cd $TARGET
#ln -s /usr/bin/qmake6 ./qmake
pwd
dpkg-buildpackage -b -uc -us -d