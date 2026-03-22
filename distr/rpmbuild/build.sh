#!/bin/bash -e

CURRENT_DIR=$(pwd)
QT=
if [ "x"$1 != "x" ] ; then
#  cd ..
  QT=-qt5
fi

RPM_ROOT="$CURRENT_DIR/rpm${QT}"


rm -rf $RPM_ROOT

# 1. 
mkdir -p "$RPM_ROOT"/{BUILD,RPMS,SOURCES,SPECS,SRPMS}

# 2. 

cp avrpioremote${QT}.spec "$RPM_ROOT/SPECS/"

mkdir -p $RPM_ROOT/SOURCES
cp -r ../../src $RPM_ROOT/SOURCES
cp -r ../../*.pro $RPM_ROOT/SOURCES
#cp -r ../../src/*.ini $RPM_ROOT/SOURCES
cp -r ../../src/theme $RPM_ROOT/SOURCES
cp -r ../debian/*.desktop $RPM_ROOT/SOURCES
cp -r ../Qt5/debian/*.desktop $RPM_ROOT/SOURCES
export APP_VERSION_VALUE=26.03
# 3. 
rpmbuild -ba \
  --define "_topdir $RPM_ROOT" \
  --build-in-place \
  avrpioremote${QT}.spec

echo "========================================"
echo "Complete. Find your packages in: $RPM_ROOT/RPMS/"