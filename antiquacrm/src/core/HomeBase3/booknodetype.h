// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef LIB_BOOKNODE_TYPE_H
#define LIB_BOOKNODE_TYPE_H

#include "homebaseconfig.h"

class HOMEBASE3_EXPORT BookNodeType final {
private:
  QString p_name;
  QMetaType::Type p_type;
  int p_minLength;
  int p_maxLength;

public:
  explicit BookNodeType(const QString &typeName, QMetaType::Type mType,
                        int min = 0, int max = 30);
  BookNodeType(const BookNodeType &p);
  BookNodeType();
  const QString name() const;
  const QMetaType::Type type() const;
  int minLength() const;
  int maxLength() const;
};

#endif // LIB_BOOKNODE_TYPE_H
