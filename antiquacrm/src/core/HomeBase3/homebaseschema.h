// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef LIB_HOMEBASE_SCHEMA_H
#define LIB_HOMEBASE_SCHEMA_H

#include <QDomDocument>
#include <QDomElement>
#include <QFileInfo>
#include <QMetaType>
#include <QString>

#include "homebaseconfig.h"
#include "booknodetype.h"

class HOMEBASE3_EXPORT HomeBaseSchema final : public QDomDocument {
private:
  QDomElement p_element;

  /**
   * @brief Find BookInventory Schemafile
   * Search for /xml/BookInventory.xsd in QStandardPaths::DataLocation and
   * QDir::currentPath().
   * @return filePath
   */
  const QString schemaFile() const;

  int getLength(const QDomElement &e, const QString &section);

  const BookNodeType fetchType(const QDomElement &element,
                           const QString &section = QString("simpleType"));

  bool loadSchema();

public:
  explicit HomeBaseSchema();
  const BookNodeType getPolicy(const QDomElement &base);
};

#endif // LIB_HOMEBASE_SCHEMA_H
