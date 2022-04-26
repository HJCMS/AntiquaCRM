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


if test -d ./${_subdir} ; then

_filename=$(echo ${_class} | sed -e 's/ //' | awk '{print tolower($1)}')
_MACRO=$(echo "${_filename}_${_subdir}" | awk '{print toupper($1)}')

echo "./${_subdir}/${_filename}.h"

cat > ./${_subdir}/${_filename}.h <<EOF
// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ${_MACRO}_H
#define ${_MACRO}_H

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

#include <UtilsMain>

class ${_class} : public UtilsMain
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_info;

// Q_SIGNALS:

public Q_SLOTS:
  void setValue(const QVariant &);
  void reset();

public:
  explicit ${_class}(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
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

${_class}::${_class}(QWidget *parent) : UtilsMain{parent} {
  if(objectName().isEmpty())
    setObjectName("${_class}");

  setModified(false);
  setRequired(false);

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_info = new QLabel(this);
  layout->addWidget(m_info);

  qDebug() << Q_FUNC_INFO << "TODO";

  setLayout(layout);
}

void ${_class}::setValue(const QVariant &)
{
  qDebug() << Q_FUNC_INFO << "TODO";
}

void ${_class}::reset()
{
  qDebug() << Q_FUNC_INFO << "TODO";
  setModified(false);
}

const QVariant ${_class}::value()
{}

bool ${_class}::isValid()
{
  qDebug() << Q_FUNC_INFO << "TODO";
// if(isRequired() && )
//  return false;

  return true;
}

void ${_class}::setInfo(const QString &info)
{
  return m_info->setToolTip(info);
  return m_info->setText(info);
}

const QString ${_class}::info()
{
  return m_info->text();
}

const QString ${_class}::notes()
{
  qDebug() << Q_FUNC_INFO << "TODO";
  // return tr("");
}

EOF

echo "done"
fi
