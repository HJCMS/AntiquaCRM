// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklisting.h"

BookListing::BookListing(const QDomElement &element) : QDomElement{element} {}

const QStringList BookListing::defaultNodeNames() {
  QStringList nl;
  nl << "vendorListingid";
  nl << "title";
  nl << "author";
  nl << "illustrator";
  nl << "editor";
  nl << "price";
  nl << "quantity";
  nl << "shippingMultiplier";
  nl << "publisher";
  nl << "description";
  nl << "subject";
  nl << "bookCondition";
  nl << "jacketCondition";
  nl << "bookType";
  nl << "size";
  nl << "edition";
  nl << "inscriptionType";
  nl << "volumeText";
  nl << "printingText";
  nl << "language";
  nl << "weight";
  nl << "synopsis";
  nl << "universalIdentifier";
  nl << "abeCategory";
  nl << "buyerSearchAttribute";
  nl << "picture";
  nl << "resellerExclusion";
  nl << "vendorCatalog";
  nl << "customField";
  return nl;
}

const QDomElement BookListing::getElement(const QString &nodeName) const {
  QDomElement dummy;
  if (!defaultNodeNames().contains(nodeName)) {
    qWarning("Unregistered nodeName!");
    return dummy.toElement();
  }

  QDomNodeList nodes = childNodes();
  for (int i = 0; i < nodes.count(); i++) {
    QDomElement e = nodes.at(i).toElement();
    if (e.nodeName() == nodeName)
      return e;
  }

  return dummy;
}

void BookListing::setListingId(qint64 listingId) {
  QString value = QString::number(listingId);
  QDomAttr attr = attributeNode("listingsid");
  if (attr.specified()) {
    attr.setValue(value);
  } else {
    setAttribute("listingsid", value);
  }
}

qint64 BookListing::listingsid() const {
  QString id = attribute("listingsid", "0");
  bool convert;
  qint64 value = id.toLongLong(&convert);
  if (convert)
    return value;

  return 0;
}

void BookListing::setTypeCodec(int type) {
  QString value = QString::number(type);
  QDomAttr attr = attributeNode("typecode");
  if (attr.specified()) {
    attr.setValue(value);
  } else {
    setAttribute("typecode", value);
  }
}

int BookListing::typecode() const {
  QString id = attribute("typecode", "0");
  bool convert;
  int value = id.toInt(&convert);
  if (convert)
    return value;

  return 0;
}

void BookListing::setDeleteStatus(bool status) {
  QString value = (status) ? "true" : "false";
  QDomAttr attr = attributeNode("deletestatus");
  if (attr.specified()) {
    attr.setValue(value);
  } else {
    setAttribute("deletestatus", value);
  }
}

bool BookListing::deletestatus() const {
  return (attribute("deletestatus", "false") == "true");
}

void BookListing::setUpdateDate(const QDateTime &dateTime) {
  // https://www.w3.org/TR/xmlschema-2/#dateTime
  QString value = dateTime.toString(HOMEBASE3_DATETIME_FORMAT);
  QDomAttr attr = attributeNode("updatedate");
  if (attr.specified()) {
    attr.setValue(value);
  } else {
    setAttribute("updatedate", value);
  }
}

QDateTime BookListing::updatedate() const {
  QString d = QDateTime::currentDateTime().toString(HOMEBASE3_DATETIME_FORMAT);
  return QDateTime::fromString(attribute("deletestatus", d),
                               HOMEBASE3_DATETIME_FORMAT);
}
