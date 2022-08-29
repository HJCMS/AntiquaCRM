// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef LIB_HOMEBASE_BUILDER_H
#define LIB_HOMEBASE_BUILDER_H

#include <QDomDocument>
#include <QString>

#include "homebaseconfig.h"
#include "booklisting.h"

class HOMEBASE3_EXPORT HomeBaseBuilder final : public QDomDocument {
private:
  const QString p_xmlns;
  const QString p_xmlns_xsi;
  QDomElement rootNode;

  BookListing createListingNode();

  /**
   * @brief construct Document root
   */
  bool initDocument();

public:
  explicit HomeBaseBuilder(const QString &name = "BookListingInventory");

  /**
   * @brief Create Document
   */
  bool createDocument();

  /**
   * @brief Write Document To File given to Destination
   */
  bool writeToFile(const QString &destDir);
};

#endif // LIB_HOMEBASE_BUILDER_H
