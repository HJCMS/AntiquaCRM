# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

## rpmbuild -ba SPECS/antiquacrm.spec

## Minimum required Qt Version
%define qtversion 5.15.2

Name:           antiquacrm
Summary:        @DESCRIPTION@
Version:        @ANTIQUACRM_VERSION_STRING@
Release:        @ANTIQUACRM_PACKAGE_RELEASE@
License:        GPLv2 GPLv3
AutoReqProv:    on
Source0:        %{_sourcedir}/@PROJECTNAME@-%{version}.tar.xz
Source1:        %{_sourcedir}/qtbase_de.ts
Group:          Productivity/Databases
Url:            @HOMEPAGEURL@
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
BuildRequires:  cmake >= 3.17.0 git
## BuildRequires:  libcurl-devel
BuildRequires:  qrencode-devel
BuildRequires:  libQt5Core-devel >= %{qtversion}
BuildRequires:  libQt5Gui-devel >= %{qtversion}
BuildRequires:  libQt5Widgets-devel >= %{qtversion}
BuildRequires:  libQt5Xml-devel >= %{qtversion}
BuildRequires:  libQt5Sql-devel >= %{qtversion}
BuildRequires:  libQt5Network-devel >= %{qtversion}
BuildRequires:  libqt5-linguist-devel >= %{qtversion}
BuildRequires:  libQt5PrintSupport-devel >= %{qtversion}
BuildRequires:  libQt5Charts5-devel >= %{qtversion}

%debug_package

%description
It offers item inventory management and a number of online interfaces for simplified data management for individual service providers.

%package cron
Summary:        Server CronJob Application
Group:          Productivity/Databases
Requires:       cron

%description cron
Application that call Providers Orsders and add Customers and Orders into the Database.

%package devel
Summary:        @DESCRIPTION@
Group:          Development/Languages/C and C++
Requires:       antiquacrm >= %{version}

%description devel
It offers item inventory management and a number of online interfaces for simplified data management for individual service providers.

%prep
%setup -q

if test -d .git ; then
  git pull
fi

%__mkdir_p build build_cron

%build

cp %{_sourcedir}/qtbase_de.ts application/src/i18n/

cd build
cmake -Wno-dev -Wno-deprecated \
  -DCMAKE_BUILD_TYPE:STRING=MinSizeRel \
  -DCMAKE_CXX_FLAGS_MINSIZEREL:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_INSTALL_PREFIX:PATH=%{_prefix} \
  -DLIB_SUFFIX:STRING=64 \
  -DCMAKE_SKIP_RPATH:BOOL=ON \
  ../application/

%__make

cd ../build_cron
cmake -Wno-dev -Wno-deprecated \
  -DCMAKE_BUILD_TYPE:STRING=MinSizeRel \
  -DCMAKE_CXX_FLAGS_MINSIZEREL:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_INSTALL_PREFIX:PATH=%{_prefix} \
  -DLIB_SUFFIX:STRING=64 \
  -DCMAKE_SKIP_RPATH:BOOL=ON \
  ../server/

%__make

%install
pushd build
  %makeinstall
popd

pushd build_cron
  install -m 0755 src/antiquacmd %{?buildroot:%{buildroot}}/%{_bindir}/antiquacmd
popd

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%defattr(-, root, root)
## binaries
%{_bindir}/antiquacrm
%{_bindir}/antiquacrm_assistant
%{_libdir}/libAntiquaCRM.so.1*
%{_libdir}/libAntiquaCRMPlugin.so.1*
%dir %{_libdir}/antiquacrm
%dir %{_libdir}/antiquacrm/plugins
%dir %{_libdir}/antiquacrm/plugins/provider
%{_libdir}/antiquacrm/plugins/provider/lib*.so
%{_datadir}/pixmaps/antiquacrm.png
%dir %{_datadir}/antiquacrm
%dir %{_datadir}/antiquacrm/i18n
%dir %{_datadir}/antiquacrm/data
%dir %{_datadir}/antiquacrm/data/pgsql
%dir %{_datadir}/antiquacrm/data/json
%dir %{_datadir}/antiquacrm/data/fonts
%doc %{_mandir}/man1/antiquacrm.1*
%{_datadir}/antiquacrm/LICENSE
%{_datadir}/antiquacrm/i18n/antiquacrm*.qm
%{_datadir}/antiquacrm/data/pgsql/*.sql
%{_datadir}/antiquacrm/data/json/*.json
%{_datadir}/antiquacrm/data/fonts/*.ttf
%{_datadir}/applications/de.hjcms.antiquacrm.desktop
%{_datadir}/applications/de.hjcms.antiquacrm_assistant.desktop

%files cron
%defattr(-, root, root)
%{_bindir}/antiquacmd

%post devel
/sbin/ldconfig

%postun devel
/sbin/ldconfig

%files devel
%defattr(-, root, root)
%{_libdir}/libAntiquaCRM.so
%{_libdir}/libAntiquaCRMPlugin.so
%dir %{_includedir}/AntiquaCRM
%{_includedir}/AntiquaCRM/*.h
%{_includedir}/AntiquaCRM/AGlobal
%{_includedir}/AntiquaCRM/ASettings
%{_includedir}/AntiquaCRM/AntiquaCRM
%{_includedir}/AntiquaCRM/AntiquaCRMPlugin
%{_datadir}/antiquacrm/antiquacrm_untranslated.ts

%clean
rm -rf %{buildroot}

%changelog
#eof
