## rpmbuild -ba SPECS/AntiquaCRM.spec

%define qtversion 5.15.2

Name:           antiquacrm
Summary:        Antiquarische Datenbank Verwaltung
Version:        0.1.10
Release:        1
License:        GPLv2 GPLv3
AutoReqProv:    on
## git clone https://github.com/HJCMS/AntiquaCRM.git antiquacrm-0.1.10
Source0:        %{_sourcedir}/antiquacrm-%{version}.tar.xz
Group:          Productivity/Databases
Url:            http://www.hjcms.de
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
## rpmm -ehv libQt5Core-devel libQt5Gui-devel libQt5Widgets-devel libQt5Xml-devel libQt5Sql-devel libQt5Network-devel libqt5-linguist-devel libQt5PrintSupport-devel
BuildRequires:  cmake >= 3.17.0
BuildRequires:  libQt5Core-devel >= %{qtversion}
BuildRequires:  libQt5Gui-devel >= %{qtversion}
BuildRequires:  libQt5Widgets-devel >= %{qtversion}
BuildRequires:  libQt5Xml-devel >= %{qtversion}
BuildRequires:  libQt5Sql-devel >= %{qtversion}
BuildRequires:  libQt5Network-devel >= %{qtversion}
BuildRequires:  libqt5-linguist-devel >= %{qtversion}
BuildRequires:  libQt5PrintSupport-devel >= %{qtversion}
BuildRequires:  libcurl-devel qrencode-devel git

%description
Ist ein Programm zur Verwaltung von Antiquarischen Büchern.

Es bietet eine Verwaltung der Artikelbestände und einige Online Schnittstellen für eine vereinfachte Datenverwaltung zu einzelnen Dienstanbietern.

Es stellt z.B. ISBN Abfragen an OpenLibrary.org und ermöglicht es somit Dateneingaben zu prüfen.

Das Programm wird mit der Qt-C++ Version 5.12.* Community Lizens geschrieben und verwendet eine PostgreSQL Datenbank ab Version 14.* für die Datenverwaltung.


%package devel
Summary:        Antiquarische Datenbank Verwaltung
Group:          Development/Languages/C and C++
Requires:       antiquacrm >= %{version}

%description devel
Ist ein Programm zur Verwaltung von Antiquarischen Büchern.

Es bietet eine Verwaltung der Artikelbestände und einige Online Schnittstellen für eine vereinfachte Datenverwaltung zu einzelnen Dienstanbietern.

Es stellt z.B. ISBN Abfragen an OpenLibrary.org und ermöglicht es somit Dateneingaben zu prüfen.

Das Programm wird mit der Qt-C++ Version 5.12.* Community Lizens geschrieben und verwendet eine PostgreSQL Datenbank ab Version 14.* für die Datenverwaltung.


%prep
%setup -q

git pull

%__mkdir_p build

%build

cd build
cmake -Wno-dev -Wno-deprecated \
  -DCMAKE_BUILD_TYPE:STRING=MinSizeRel \
  -DCMAKE_CXX_FLAGS_MINSIZEREL:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_INSTALL_PREFIX:PATH=%{_prefix} \
  -DLIB_SUFFIX:STRING=64 \
  -DANTIQUA_LSB_INSTALL:BOOL=ON \
  -DCMAKE_SKIP_RPATH:BOOL=ON \
  -DCURL_DIR:PATH=%{_prefix} \
  ../antiquacrm/

%__make

%install

pushd build
  %makeinstall
popd

%post
ldconfig

%postun
ldconfig

%files
%defattr(-, root, root)
%{_bindir}/%{name}
%{_datadir}/%{name}
%dir %{_libdir}/antiqua
%dir %{_libdir}/antiqua/plugins
%{_libdir}/antiqua/plugins/lib*.so
%{_libdir}/libAntiquaCore.so.0*
%{_datadir}/applications/de.hjcms.antiquacrm.desktop

%files devel
%defattr(-, root, root)
%{_libdir}/libAntiquaCore.so
%dir %{_includedir}/antiquacrm
%{_includedir}/antiquacrm/*

%clean
rm -rf %{buildroot}

%changelog
#eof
