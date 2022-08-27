// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef LIB_HOMEBASE_BUILDER_H
#define LIB_HOMEBASE_BUILDER_H

#include <QString>
#include <QDomDocument>

#include "homebaseconfig.h"

class HOMEBASE3_EXPORT HomeBaseBuilder final : public QDomDocument
{
private:
  QDomElement rootNode;

public:
  explicit HomeBaseBuilder();
};

#endif // LIB_HOMEBASE_BUILDER_H
