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

class HOMEBASE3_EXPORT BookListingNode : public QDomElement {
public:
  explicit BookListingNode(const QDomElement &parent);

  /**
   * @brief DomElement NodeName
   * @return String
   */
  virtual const QString name() const = 0;

  /**
   * @brief Meta Type definition
   * @return MetaType
   */
  virtual QMetaType::Type type() const = 0;

  /**
   * @brief minimum String Length or Size
   * @return int
   */
  virtual int minLength() const = 0;

  /**
   * @brief maxLength
   * @return int
   */
  virtual int maxLength() const = 0;

  /**
   * @brief set NodeValue
   */
  void setValue(const QVariant &value);

  /**
   * @brief nodeValue
   * @return Variant
   */
  const QVariant value() const;

  /**
   * @brief Convert to JSonObject
   * @return JsonObject
   */
  const QJsonObject toJson();
};

#endif // LIB_BOOKLISTING_NODE_H
