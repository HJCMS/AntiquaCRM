#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

set +x

_class=${1}

test -n "${_class}" || {
  echo "no name for target"
  read -i "Please Insert a Valid Project name:" _class
}

_name=$(echo ${_class} | sed -e 's/ //' | awk '{print tolower($1)}')
_project=$(echo ${_name} | awk '{print toupper($1)}')

if test ! -d ./${_name} ; then

echo "create sub project target and minimal CMakeLists.txt"

mkdir -vm 0750 ./${_name}

cat > ./${_name}/${_name}.h <<EOF
// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ${_project}_H
#define ${_project}_H

#include <QtCore/QObject>
#include <QtWidgets/QWidget>

class ${_class} : public QWidget
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit ${_class}(QWidget *parent = nullptr);

};

#endif // ${_project}_H

EOF

cat > ./${_name}/${_name}.cpp <<EOF
// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "version.h"
#include "${_name}.h"

#include <QtCore/QDebug>

${_class}::${_class}(QWidget *parent) : QWidget{parent} {
  setObjectName("${_class}");
}

EOF

cat > ./${_name}/CMakeLists.txt <<EOF
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "${_class}")

INCLUDE_DIRECTORIES (
  \${PROJECT_INCLUDE_DIRS}
  \${CMAKE_CURRENT_BINARY_DIR}
  \${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_FILES
)

SET (subproject_SOURCES
  ${_name}.cpp
)

SET (subproject_HEADERS
  ${_name}.h
)

ADD_LIBRARY (\${_subproject} STATIC
  \${subproject_HEADERS}
  \${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (\${_subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME \${_subproject}
)

TARGET_LINK_LIBRARIES (\${_subproject} PRIVATE
  Qt\${QT_VERSION_MAJOR}::Core
  Qt\${QT_VERSION_MAJOR}::Widgets
)

EOF

fi
