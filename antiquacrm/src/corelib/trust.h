// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TRUST_ANTIQUA_CORE_H
#define TRUST_ANTIQUA_CORE_H

#include <QtCore/QObject>
#include <QtCore/QString>

/**
 * @brief Trust Class for SQL c_trusted Field
 */
QT_SHARED class Trust {
private:
  int p_index;
  QString p_title;

public:
  explicit Trust();
  Trust(int index, const QString &title);
  int index();
  const QString title();
};
Q_DECLARE_TYPEINFO(Trust, Q_PRIMITIVE_TYPE);

#endif // TRUST_ANTIQUA_CORE_H
