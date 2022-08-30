// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef LIB_HOMEBASE_TITLENODE_H
#define LIB_HOMEBASE_TITLENODE_H

#include "booklistingnode.h"

class HOMEBASE3_EXPORT TitleNode final : public BookListingNode {
public:
  explicit TitleNode(const QDomElement &parent);
  const QString name() const;
  QMetaType::Type type() const;
  int minLength() const;
  int maxLength() const;
};

#endif // LIB_HOMEBASE_TITLENODE_H
