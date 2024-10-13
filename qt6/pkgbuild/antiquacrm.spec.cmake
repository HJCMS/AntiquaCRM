# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

## rpmbuild -ba SPECS/antiquacrm.spec

## Minimum required Qt Version
%define qtversion 6.2.4

Name:           antiquacrm
Summary:        @DESCRIPTION@
Version:        @ANTIQUACRM_VERSION_STRING@
Release:        @ANTIQUACRM_PACKAGE_RELEASE@
License:        GPLv2 GPLv3
AutoReqProv:    on
Source0:        %{_sourcedir}/@PROJECTNAME@-%{version}.tar.xz
Group:          Productivity/Databases
Url:            @HOMEPAGEURL@
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
Requires:       libAntiquaCRM1 = %{version}-%{release}
Requires:       libAntiquaCRM1 = %{version}-%{release}
Requires:       qt6-sql-postgresql >= %{qtversion}
Requires:       libdiscid0 >= 0.6.2
Requires:       libqrencode4 >= 4.1.1
## https://documentation.suse.com/sbp/all/html/SBP-RPM-Packaging/index.html
Suggests:       %{name}-assistant
BuildRequires:  qt6-core-devel >= %{qtversion}
BuildRequires:  qt6-gui-devel >= %{qtversion}
BuildRequires:  qt6-widgets-devel >= %{qtversion}
BuildRequires:  qt6-charts-devel >= %{qtversion}
BuildRequires:  qt6-xml-devel >= %{qtversion}
BuildRequires:  qt6-sql-devel >= %{qtversion}
BuildRequires:  qt6-network-devel >= %{qtversion}
BuildRequires:  qt6-printsupport-devel >= %{qtversion}
BuildRequires:  qt6-tools-devel >= %{qtversion}
BuildRequires:  qt6-tools-linguist >= %{qtversion}
BuildRequires:  libdiscid-devel >= 0.6.2
BuildRequires:  qrencode-devel >= 4.1.1
## glslc:  shaderc shaderc-devel
BuildRequires:  xproto-devel xcb-proto-devel
BuildRequires:  xcb-util-devel libxkbcommon-x11-devel libxkbfile-devel
BuildRequires:  libXss-devel libXmu-devel libXcomposite-devel libXcursor-devel

%debug_package

%description
It offers item inventory management and a number of online interfaces for simplified data management for individual service providers.

%package -n libAntiquaCRM1
Summary:        Core Library package
Group:          Development/Libraries
Requires:       qt6-sql-postgresql >= %{qtversion}

%description -n libAntiquaCRM1
Core Library for AntiquaCRM applications

%package assistant
Summary:        Application assistant for first usage.
Group:          Productivity/Databases
Provides:       antiqua_assistant
Requires:       libAntiquaCRM1 = %{version}-%{release}

%description assistant
Application assistant for first usage.

%package cron
Summary:        CronJob Programm
Group:          Productivity/Databases
Requires:       cron
Requires:       libAntiquaCRM1 = %{version}-%{release}

%description cron
Application that call Providers Orsders and add Customers and Orders into the Database.

%package devel
Summary:        Antiquarische Datenbank Verwaltung
Group:          Development/Languages/C and C++
Requires:       libAntiquaCRM1 = %{version}-%{release}
Requires:       qrencode-devel libdiscid-devel >= 0.6.2
Requires:       qt6-core-devel >= %{qtversion}
Requires:       qt6-gui-devel >= %{qtversion}
Requires:       qt6-widgets-devel >= %{qtversion}
Requires:       qt6-charts-devel >= %{qtversion}
Requires:       qt6-xml-devel >= %{qtversion}
Requires:       qt6-sql-devel >= %{qtversion}
Requires:       qt6-network-devel >= %{qtversion}
Requires:       qt6-printsupport-devel >= %{qtversion}
Requires:       qt6-tools-devel >= %{qtversion}

%description devel
It offers item inventory management and a number of online interfaces for simplified data management for individual service providers.

%prep
%setup -q

if test -d .git ; then
  git pull
  git submodule init
  git submodule update
else
  mkdir -p  qt6/src/i18n/qt6-tr/translations/
  cp %{_sourcedir}/qtbase_de.ts qt6/src/i18n/qt6-tr/translations/
fi

%__mkdir_p build

%build

cd build
cmake -Wno-dev -Wno-deprecated \
  -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++%{std_standard} \
  -DCMAKE_CXX_COMPILER_AR:FILEPATH=/usr/bin/gcc-ar%{std_standard} \
  -DCMAKE_BUILD_TYPE:STRING=MinSizeRel \
  -DCMAKE_CXX_FLAGS_MINSIZEREL:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_INSTALL_PREFIX:PATH=%{_prefix} \
  -DLRELEASE_EXECUTABLE:FILEPATH=/usr/bin/lrelease6 \
  -DLIB_SUFFIX:STRING=64 \
  -DWITH_ANTIQUACMD:BOOL=ON \
  -DCMAKE_SKIP_RPATH:BOOL=ON \
  ../qt6/

make --jobs 6

%install
pushd build
  %makeinstall
popd

%post -n libAntiquaCRM1
/sbin/ldconfig

%postun -n libAntiquaCRM1
/sbin/ldconfig

%files -n libAntiquaCRM1
%defattr(-, root, root)
%dir %{_libdir}/antiquacrm
%dir %{_libdir}/antiquacrm/plugins
%dir %{_libdir}/antiquacrm/plugins/providers
%dir %{_libdir}/antiquacrm/plugins/tabs
%dir %{_libdir}/antiquacrm/plugins/acmdproviders
%{_libdir}/libAntiqua*.so.1*

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%defattr(-, root, root)
## binaries
%{_bindir}/antiquacrm
%dir %{_libdir}/antiquacrm/plugins/providers/Buchfreund
%dir %{_libdir}/antiquacrm/plugins/providers/Booklooker
%dir %{_libdir}/antiquacrm/plugins/providers/AbeBooks
%{_libdir}/antiquacrm/plugins/providers/*/lib*.so
%dir %{_libdir}/antiquacrm/plugins/tabs/Books
%dir %{_libdir}/antiquacrm/plugins/tabs/CDsVinyl
%dir %{_libdir}/antiquacrm/plugins/tabs/Reports
%dir %{_libdir}/antiquacrm/plugins/tabs/Statistics
%dir %{_libdir}/antiquacrm/plugins/tabs/Stitches
%dir %{_libdir}/antiquacrm/plugins/tabs/Various
%dir %{_libdir}/antiquacrm/plugins/tabs/Views
%{_libdir}/antiquacrm/plugins/tabs/*/lib*.so
%{_datadir}/pixmaps/antiquacrm.png
%dir %{_datadir}/antiquacrm
%dir %{_datadir}/antiquacrm/i18n
%dir %{_datadir}/antiquacrm/data
%dir %{_datadir}/antiquacrm/data/documents
%dir %{_datadir}/antiquacrm/data/pgsql
%dir %{_datadir}/antiquacrm/data/json
%dir %{_datadir}/antiquacrm/data/fonts
%{_datadir}/antiquacrm/data/antiquacrm.qcss
%{_datadir}/antiquacrm/i18n/antiquacrm*.qm
%{_datadir}/antiquacrm/data/pgsql/*.sql
%{_datadir}/antiquacrm/data/json/*.json
%{_datadir}/antiquacrm/data/fonts/*.ttf
%{_datadir}/antiquacrm/data/documents/*.txt
%{_datadir}/applications/de.hjcms.antiquacrm.desktop

%files assistant
%defattr(-, root, root)
%{_bindir}/antiqua_assistant
%{_datadir}/applications/de.hjcms.antiquacrm_assistant.desktop

%files cron
%defattr(-, root, root)
%{_libdir}/antiquacrm/antiquacmd
%dir %{_libdir}/antiquacrm/plugins/acmdproviders/CmdAbeBooks
%dir %{_libdir}/antiquacrm/plugins/acmdproviders/CmdBooklooker
%dir %{_libdir}/antiquacrm/plugins/acmdproviders/CmdBuchfreund
%{_libdir}/antiquacrm/plugins/acmdproviders/*/lib*.so
%dir %{_datadir}/antiquacrm/data/cron
%{_datadir}/antiquacrm/data/cron/antiqua-*

%post devel
/sbin/ldconfig

%postun devel
/sbin/ldconfig

%files devel
%defattr(-, root, root)
%{_libdir}/libAntiqua*.so
%dir %{_includedir}/AntiquaCRM
%{_includedir}/AntiquaCRM/*.h
%{_includedir}/AntiquaCRM/AGlobal
%{_includedir}/AntiquaCRM/ASettings
%{_includedir}/AntiquaCRM/AntiquaCRM
%{_includedir}/AntiquaCRM/AntiquaInput
%{_includedir}/AntiquaCRM/AntiquaWidgets
%{_includedir}/AntiquaCRM/AntiquaButtons
%{_includedir}/AntiquaCRM/AntiquaMail
%{_includedir}/AntiquaCRM/AntiquaProviders
%{_includedir}/AntiquaCRM/AntiquaTabs
%{_datadir}/antiquacrm/antiquacrm_untranslated.ts

%clean
rm -rf %{buildroot}

%changelog
#eof
