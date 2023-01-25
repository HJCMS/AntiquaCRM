# AntiquaCRM Installation

### Important notes

AntiquaCRM requires a connection to existing PostgreSQL Server v14.*.
  - AntiquaCRM did not Install Database Structures!
  - The Server Application is written for Linux and Windows Server not supported!
  - This work will separately done by Administrators!

The Database connection from outside to the local Network PgSQL-Server only works with SSL/TLS and configured Port forwarding.
  - So it requires a Router with Dyn-DNS and Port forwarding support!
  - This depends on your network service provider!

## Install instructions

For Compiling dependency please read the [link](PACKETBUILDERS.md "PACKETBUILDERS.md") file.

- Checkout this repository:
```
  git clone git@github.com:HJCMS/AntiquaCRM.git
  git submodule init
  git submodule update
```

- Compile the project:
  - cd AntiquaCRM
  - mkdir build
  - cd build
  - cmake ../application
  - make

- RPM Packetbuilding:
```
 cd ~/rpmbuild/SOURCES
 git clone --recurse-submodules git@github.com:HJCMS/AntiquaCRM.git antiquacrm-${version}
 tar -cJf antiquacrm-${version}.tar.xz antiquacrm-${version}
```

  - Generate the RPM-Specfile:
```
 mkdir antiquacrm-${version}/build
 cd antiquacrm-${version}/build
 cmake -DLIB_SUFFIX:STRING=64 ../application
 cd ~/rpmbuild
 cp SOURCES/antiquacrm-${version}/build/xdg/antiquacrm.spec SPECS/
```

  - Customize/Create the Package:
```
 vim SPECS/antiquacrm.spec
 rpmbuild -ba SPECS/antiquacrm.spec
 rpm --test -Uhv ~/rpmbuild/RPMS/x86_64/antiquacrm-${version}.rpm
```

- Windows Paketbuilding:
  - Install git and the Qt5.12* Developer Framework for Windows and the required dependency libraries.
  - If you get errors, make sure that all depends libraries in your PATH Variable.
  - Change to your working directory and chekout the project
  - git clone git@github.com:HJCMS/AntiquaCRM.git
  - Open QtCreator and import AntiquaCRM Projectfile
  - Configure cmake with MinSizeRelease.
  - Compile it ...
  - AntiquaCRM create a INNO Setup Installer File in the Build Directory.
  - Open it with inno setup and configure unspecified Variables.
  - start compile package
  - if no errors you can test the installation
  - ...

---

Links to the external projects:

1. <a href="https://www.qt.io/product/development-tools">Qt Creator</a>
2. <a href="https://jrsoftware.org/isinfo.php">Inno Setup</a>
