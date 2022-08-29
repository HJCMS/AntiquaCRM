// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef LIB_BOOKLISTING_H
#define LIB_BOOKLISTING_H

#include "homebaseconfig.h"

#include <QDateTime>
#include <QDomElement>

class HOMEBASE3_EXPORT BookListing final : public QDomElement {

public:
  explicit BookListing(const QDomElement &e);
  explicit BookListing();

  static const QStringList defaultNodeNames();

  const QDomElement getElement(const QString &nodeName) const;

  void setListingId(qint64 listingId);
  qint64 listingsid() const;

  void setTypeCodec(int type);
  int typecode() const;

  void setDeleteStatus(bool status = false);
  bool deletestatus() const;

  void setUpdateDate(const QDateTime &dateTime = QDateTime::currentDateTime());
  QDateTime updatedate() const;
};

#endif // LIB_BOOKLISTING_H
