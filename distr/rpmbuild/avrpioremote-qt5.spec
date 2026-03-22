Name:           avrpioremote-qt5
Version:        26.03
Release:        1%{?dist}
Summary:        AvrPioRemote Qt5 application

URL:            https://github.com/AceOfSnakes/AvrPioRemote

BuildArch:      x86_64
License:        GPLv3

%description
This is program for view or make some changes in firmware for MTK devices

%define _rpmfilename %%{NAME}_%%{VERSION}_%%{ARCH}.rpm

%prep
# Do NOT assume any directory; extract tarball manually
#%setup -q -T
#tar -xzf %{SOURCE0} -C .
echo =============================================
#%{__rm} -rf %{name}-%{version}
#%{__mkdir} -p %{name}-%{version}

%build
# Nothing to build; we are packaging a prebuilt binary
echo ======== $(pwd)
echo PWD: $(pwd)
cd rpm-qt5/SOURCES
qmake AVRPioRemote.pro
%{__make}

mkdir -p %{buildroot}

#cp -a settings/* %{buildroot}/
#cp -a MtkPio %{buildroot}/

%install
#rm -rf %{buildroot}
# Copy all files from tarball into build root
#cp -a * %{buildroot}/

# mkdir -p %{buildroot}%{_bindir}
mkdir -p %{buildroot}/opt/avrpioremote-qt5
mkdir -p %{buildroot}/usr/share/applications/

# make install INSTALL_ROOT=%{buildroot}
# install -m 755 NetRC %{buildroot}%{_bindir}/NetRC

mkdir -p %{buildroot}
echo =========== Install Filder %{__pwd}

#cp -r rpm/SOURCES/settings %{buildroot}/opt/NetRC/settings/
cp -a rpm-qt5/SOURCES/AVRPioRemote %{buildroot}/opt/avrpioremote-qt5
cp -r rpm-qt5/SOURCES/theme %{buildroot}/opt/avrpioremote-qt5/theme
#cp -a rpm-qt5/SOURCES/*.ini %{buildroot}/opt//mtkpio-qt5/
cp -a rpm-qt5/SOURCES/src/images/AVRPioRemote.png %{buildroot}/opt/avrpioremote-qt5/AVRPioRemote.png
cp -a rpm-qt5/SOURCES/src/*.json %{buildroot}/opt/avrpioremote-qt5/
cp -a rpm-qt5/SOURCES/avrpioremote-qt5.desktop %{buildroot}/usr/share/applications/avrpioremote-qt5.desktop

%files
/opt/avrpioremote-qt5/AVRPioRemote
/opt/avrpioremote-qt5/AVRPioRemote.png
#/opt/avrpioremote.ini
/opt/avrpioremote-qt5/theme
/opt/avrpioremote-qt5/*.json
/usr/share/applications/avrpioremote-qt5.desktop

%clean
%{__rm} -rf $RPM_BUILD_ROOT
%{__rm} -rf $RPM_BUILD_DIR/*

%changelog
* Sun Mar 01 2026 Ace Of Snakes <AceOfSnakes@gmail.com> - 26.03-1
- Initial RPM release
