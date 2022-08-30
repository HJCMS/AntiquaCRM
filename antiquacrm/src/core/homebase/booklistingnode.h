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
#include "booknodetype.h"

class HOMEBASE3_EXPORT BookListingNode final : public QDomElement {
public:
  explicit BookListingNode(const QDomElement &element);

  /**
   * @brief DomElement NodeName
   */
  const QString name();

  /**
   * @brief Properties for this Element
   */
  const BookNodeType nodeData();

  /**
   * @brief Meta Type definition
   */
  QMetaType::Type type();

  /**
   * @brief minimum DomTextNode length
   */
  int minLength();

  /**
   * @brief maximum DomTextNode length
   */
  int maxLength();

  /**
   * @brief nodeValue
   */
  const QVariant value();

  /**
   * @brief Convert to JSonObject
   */
  const QJsonObject toJson();
};

#endif // LIB_BOOKLISTING_NODE_H
