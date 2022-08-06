# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

## rpmbuild -ba SPECS/antiquacrm.spec
## Minimum required Qt Version
%define qtversion 5.15.2

Name:           antiquacrm
Summary:        a application for managing antiquarian books
Version:        @ANTIQUACRM_VERSION_STRING@
Release:        1
License:        GPLv2 GPLv3
AutoReqProv:    on
Source0:        %{_sourcedir}/antiquacrm-%{version}.tar.xz
Group:          Productivity/Databases
Url:            http://www.hjcms.de
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
## rpm -Uhv libQt5Core-devel libQt5Gui-devel libQt5Widgets-devel libQt5Xml-devel libQt5Sql-devel libQt5Network-devel libqt5-linguist-devel libQt5PrintSupport-devel
BuildRequires:  cmake >= 3.17.0 git
BuildRequires:  libcurl-devel qrencode-devel
BuildRequires:  libQt5Core-devel >= %{qtversion}
BuildRequires:  libQt5Gui-devel >= %{qtversion}
BuildRequires:  libQt5Widgets-devel >= %{qtversion}
BuildRequires:  libQt5Xml-devel >= %{qtversion}
BuildRequires:  libQt5Sql-devel >= %{qtversion}
BuildRequires:  libQt5Network-devel >= %{qtversion}
BuildRequires:  libqt5-linguist-devel >= %{qtversion}
BuildRequires:  libQt5PrintSupport-devel >= %{qtversion}

%description
It offers item inventory management and a number of online interfaces for simplified data management for individual service providers.

%package devel
Summary:        Antiquarische Datenbank Verwaltung
Group:          Development/Languages/C and C++
Requires:       antiquacrm >= %{version}

%description devel
It offers item inventory management and a number of online interfaces for simplified data management for individual service providers.

%prep
%setup -q

if test .git ; then
  git pull
fi

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
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%defattr(-, root, root)
## binaries
%{_bindir}/antiquacrm
%{_libdir}/libAntiquaCore.so.0*
%dir %{_libdir}/antiqua
%dir %{_libdir}/antiqua/plugins
%{_libdir}/antiqua/plugins/lib*.so
## datafiles
%{_datadir}/pixmaps/antiquacrm.png
%dir %{_datadir}/antiquacrm
%dir %{_datadir}/antiquacrm/i18n
%{_datadir}/antiquacrm/i18n/antiquacrm*.qm
%dir %{_datadir}/antiquacrm/icons
%{_datadir}/antiquacrm/icons/*.png
%dir %{_datadir}/antiquacrm/xml
%{_datadir}/antiquacrm/xml/Book*.xsd
%{_datadir}/antiquacrm/xml/Book*.tpl
%{_datadir}/antiquacrm/xml/*.xml
%doc %{_datadir}/antiquacrm/AUTHORS
%doc %{_datadir}/antiquacrm/ChangeLog
%doc %{_datadir}/antiquacrm/LICENSE
%doc %{_datadir}/antiquacrm/README
%doc %{_datadir}/antiquacrm/TODO
%{_datadir}/applications/de.hjcms.antiquacrm.desktop

%post devel
/sbin/ldconfig

%postun devel
/sbin/ldconfig

%files devel
%defattr(-, root, root)
%{_libdir}/libAntiquaCore.so
%dir %{_includedir}/antiquacrm
%{_includedir}/antiquacrm/*
%{_datadir}/antiquacrm/antiquacrm_untranslated.ts

%clean
rm -rf %{buildroot}

%changelog
#eof
