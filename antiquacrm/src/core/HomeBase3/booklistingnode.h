// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef LIB_BOOKLISTING_NODE_H
#define LIB_BOOKLISTING_NODE_H

#include <QDomElement>
#include <QJsonObject>
#include <QMetaType>
#include <QString>
#include <QVariant>

#include "homebaseconfig.h"
#include "homebaseschema.h"
#include "booknodetype.h"

class HOMEBASE3_EXPORT BookListingNode final : public QDomElement {

private:
  BookNodeType p_type;
  HomeBaseSchema p_schema;

public:
  explicit BookListingNode(const QDomElement &element);

  /**
   * @brief DomElement NodeName
   * @return String
   */
  const QString name();

  /**
   * @brief Meta Type definition
   * @return MetaType
   */
  QMetaType::Type type();

  /**
   * @brief minimum String Length or Size
   * @return int
   */
  int minLength();

  /**
   * @brief maxLength
   * @return int
   */
  int maxLength();

  /**
   * @brief nodeValue
   * @return Variant
   */
  const QVariant value();

  /**
   * @brief Convert to JSonObject
   * @return JsonObject
   */
  const QJsonObject toJson();
};

#endif // LIB_BOOKLISTING_NODE_H
