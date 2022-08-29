// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef LIB_HOMEBASE_READER_H
#define LIB_HOMEBASE_READER_H

#include <QDomDocument>
#include <QDomNodeList>
#include <QString>

#include "booklisting.h"
#include "homebaseconfig.h"
#include "homebaseschema.h"

class HOMEBASE3_EXPORT HomeBaseReader final : public QDomDocument {
private:
  HomeBaseSchema p_schema;

public:
  explicit HomeBaseReader();

  /**
   * @brief Open HomeBase3 Database File
   * @param xmlFile - Database Filepath
   * @param parse   - Validate Namespace Processing
   * @return
   */
  bool open(const QString &xmlFile, bool parse = false);

  /**
   * @brief id Document ist open get rootChild Nodes
   * @return DomNodeList
   */
  const QDomNodeList bookListingNodes();

  /**
   * @brief Find a BookListing Node with "vendorListingid"
   * @param articleId
   * @return DomElement
   */
  const BookListing bookListing(const QString &articleId);
};

#endif // LIB_HOMEBASE_READER_H
