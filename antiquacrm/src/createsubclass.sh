#!/usr/bin/env bash
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

set +x

_class=${1}
_subdir=${2}

test -n "${_class}" || {
  echo "no class name"
  read -ei "Please add Classname:" _class
}

test -n "${_subdir}" || {
  echo "no target set"
  read -ei "Please add Subfolder name:" _subdir
}

_filename=$(echo ${_class} | sed -e 's/ //' | awk '{print tolower($1)}')
_uppper=$(echo ${_filename} | awk '{print toupper($1)}')
_MACRO=$(echo ${_filename}_${subdir} | awk '{print toupper($1)}')

if test -d ./${_subdir} ; then

echo "./${_subdir}/${_filename}.h"

cat > ./${_subdir}/${_filename}.h <<EOF
// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ${_MACRO}_H
#define ${_MACRO}_H

// #include <UtilsMain>

#include <QtCore/QObject>
#include <QtWidgets/QWidget>

class ${_class} : public QWidget
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:

Q_SIGNALS:

public Q_SLOTS:

public:
  explicit ${_class}(QWidget *parent = nullptr);

};

#endif // ${_MACRO}_H

EOF

echo "./${_subdir}/${_filename}.cpp"
cat > ./${_subdir}/${_filename}.cpp <<EOF
// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "${_filename}.h"
// #include "version.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

${_class}::${_class}(QWidget *parent) : QWidget{parent} {
  setObjectName("${_class}");
}
EOF

echo "done"
fi
