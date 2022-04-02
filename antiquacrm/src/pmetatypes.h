// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SQLMETATYPES_H
#define SQLMETATYPES_H

#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QString>

struct SearchStatement {
  QString SearchField;
  QString SearchString;
};
Q_DECLARE_METATYPE(SearchStatement)

#endif // SQLMETATYPES_H
