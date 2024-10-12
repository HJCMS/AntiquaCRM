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
Supplements:    %{name}-assistant
BuildRequires:  cmake >= 3.17.0 git
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

%package -n libAntiquaCRM1
Summary:        Core Library package
Group:          Development/Libraries

%description -n libAntiquaCRM1
Core Library for AntiquaCRM applications

%package assistant
Summary:        Application assistant for first usage.
Group:          Productivity/Databases
Requires:       cron
Requires:       libAntiquaCRM1 = %{version}-%{release}

%description assistant
Application assistant for first usage.

%package cron
Summary:        Server CronJob Application
Group:          Productivity/Databases
Provides:       antiquacmd
Requires:       cron
Requires:       libAntiquaCRM1 = %{version}-%{release}

%description cron
Application that call Providers Orsders and add Customers and Orders into the Database.

%package devel
Summary:        @DESCRIPTION@
Group:          Development/Languages/C and C++
Requires:       libAntiquaCRM1 = %{version}-%{release}

%description devel
It offers item inventory management and a number of online interfaces for simplified data management for individual service providers.

%prep
%setup -q

if test -d .git ; then
  git pull
fi

%__mkdir_p build

%build

cp %{_sourcedir}/qtbase_de.ts qt5/src/i18n/

cd build
cmake -Wno-dev -Wno-deprecated \
  -DCMAKE_BUILD_TYPE:STRING=MinSizeRel \
  -DCMAKE_CXX_FLAGS_MINSIZEREL:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_INSTALL_PREFIX:PATH=%{_prefix} \
  -DLIB_SUFFIX:STRING=64 \
  -DWITH_ANTIQUACMD:BOOL=ON \
  -DCMAKE_SKIP_RPATH:BOOL=ON \
  ../qt6/

%__make

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
%{_libdir}/libAntiqua*.so.1*

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%defattr(-, root, root)
## binaries
%{_bindir}/antiquacrm
%dir %{_libdir}/antiquacrm
%dir %{_libdir}/antiquacrm/plugins
%dir %{_libdir}/antiquacrm/plugins/providers
%{_libdir}/antiquacrm/plugins/providers/lib*.so
%dir %{_libdir}/antiquacrm/plugins/tabs
%{_libdir}/antiquacrm/plugins/tabs/lib*.so
%{_datadir}/pixmaps/antiquacrm.png
%dir %{_datadir}/antiquacrm
%dir %{_datadir}/antiquacrm/i18n
%dir %{_datadir}/antiquacrm/data
%dir %{_datadir}/antiquacrm/data/pgsql
%dir %{_datadir}/antiquacrm/data/json
%dir %{_datadir}/antiquacrm/data/fonts
%dir %{_datadir}/antiquacrm/data/documents
%{_datadir}/antiquacrm/LICENSE*
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
%{_bindir}/antiquacmd
%dir %{_datadir}/antiquacrm
%dir %{_datadir}/antiquacrm/scripts
%{_datadir}/antiquacrm/scripts/antiqua-*

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
%{_includedir}/AntiquaCRM/AntiquaCRMPlugin
%{_datadir}/antiquacrm/antiquacrm_untranslated.ts

%clean
rm -rf %{buildroot}

%changelog
#eof
