# AntiquaCRM

Notes for packet builders

####  Troubleshooting

If you get cmake errors like missing TS files, make sure you initialize the Qt-Comunity Translations submodule in your repository.

- Inititial Qt-Comunity Translations:
  - git submodule init
  - git submodule update

####  Dependencies

Qt5 Framework and Translations

- Required Qt5 Libraries:
  - Core
  - Widgets
  - Sql
  - Network
  - Xml
  - LinguistTools
  - PrintSupport
  - Charts

- Optional Qt5 Libraries:
  - DBus (Linux only)

- Other required Libraries:
  - PostgreSQL Libraries version 14.*
  - libqrencode
  - zlib

- Other optional Libraries:
  - libdiscid from MusicBrainz.org (Linux only) (currently no official Win64 library exists)

---

Links to the external projects:

1. <a href="https://www.qt.io/product/framework">Qt Framework</a>
2. <a href="https://doc.qt.io/qt-5/internationalization.html">Qt Translation Project</a>
3. <a href="https://musicbrainz.org/doc/Developer_Resources">MusicBrainz</a>
4. <a href="https://www.postgresql.org">PostgreSQL</a>
5. <a href="https://github.com/fukuchi/libqrencode">libqrencode</a>
