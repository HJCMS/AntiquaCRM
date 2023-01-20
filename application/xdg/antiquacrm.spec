# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

## rpmbuild -ba SPECS/antiquacrm.spec
## Minimum required Qt Version
%define qtversion 5.15.2

Name:           antiquacrm
Summary:        a application for managing antiquarian books
Version:        1.0.0
Release:        %(date +"%j")
License:        GPLv2 GPLv3
AutoReqProv:    on
Source0:        %{_sourcedir}/antiquacrm-%{version}.tar.xz
Source1:        %{_sourcedir}/qtbase_de.ts
Group:          Productivity/Databases
Url:            http://www.hjcms.de
BuildRoot:      %{_tmppath}/%{name}-%{version}-build
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

%debug_package

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

if test -d .git ; then
  git pull
fi

%__mkdir_p build

%build

cp %{_sourcedir}/qtbase_de.ts v1/src/i18n/

cd build
cmake -Wno-dev -Wno-deprecated \
  -DCMAKE_BUILD_TYPE:STRING=MinSizeRel \
  -DCMAKE_CXX_FLAGS_MINSIZEREL:STRING="$RPM_OPT_FLAGS" \
  -DCMAKE_INSTALL_PREFIX:PATH=%{_prefix} \
  -DLIB_SUFFIX:STRING=64 \
  -DCMAKE_SKIP_RPATH:BOOL=ON \
  ../v1/

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
%{_datadir}/antiquacrm/LICENSE
%{_datadir}/antiquacrm/i18n/antiquacrm*.qm
%{_datadir}/antiquacrm/data/pgsql/*.sql
%{_datadir}/antiquacrm/data/json/*.json
%{_datadir}/applications/de.hjcms.antiquacrm*.desktop

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
