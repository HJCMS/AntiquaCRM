// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklistingnode.h"
// #include "booknodetype.h"

#include <QDateTime>
#include <QDomText>
#include <QJsonValue>

BookListingNode::BookListingNode(const QDomElement &element) : QDomElement{element} {}

const QString BookListingNode::name() {
  return tagName();
}

const BookNodeType BookListingNode::nodeData() {
  QList<BookNodeType> list;
  list.append(BookNodeType("abeCategory", QMetaType::Int, 0, 0));
  list.append(BookNodeType("author", QMetaType::QString, 0, 750));
  list.append(BookNodeType("bookCondition", QMetaType::QString, 0, 30));
  list.append(BookNodeType("bookType", QMetaType::QString, 0, 10));
  list.append(BookNodeType("buyerSearchAttribute", QMetaType::QString, 0, 0));
  list.append(BookNodeType("customField", QMetaType::QString, 0, 0));
  list.append(BookNodeType("description", QMetaType::QString, 0, 4000));
  list.append(BookNodeType("edition", QMetaType::Int, 0, 4));
  list.append(BookNodeType("editor", QMetaType::QString, 0, 0));
  list.append(BookNodeType("illustrator", QMetaType::QString, 0, 255));
  list.append(BookNodeType("inscriptionType", QMetaType::QString, 0, 50));
  list.append(BookNodeType("jacketCondition", QMetaType::QString, 0, 30));
  list.append(BookNodeType("language", QMetaType::QString, 0, 3));
  list.append(BookNodeType("picture", QMetaType::QString, 0, 128));
  list.append(BookNodeType("price", QMetaType::Int, 0, 3));
  list.append(BookNodeType("printingText", QMetaType::QString, 0, 0));
  list.append(BookNodeType("publisher", QMetaType::QString, 0, 160));
  list.append(BookNodeType("publishYear", QMetaType::QString, 0, 4));
  list.append(BookNodeType("publisherName", QMetaType::QString, 0, 255));
  list.append(BookNodeType("quantity", QMetaType::Int, 0, 5));
  list.append(BookNodeType("resellerExclusion", QMetaType::QString, 0, 0));
  list.append(BookNodeType("shippingMultiplier", QMetaType::QString, 0, 0));
  list.append(BookNodeType("size", QMetaType::Int, 0, 0));
  list.append(BookNodeType("subject", QMetaType::QString, 0, 0));
  list.append(BookNodeType("synopsis", QMetaType::QString, 0, 255));
  list.append(BookNodeType("title", QMetaType::QString, 0, 255));
  list.append(BookNodeType("universalIdentifier", QMetaType::LongLong, 0, 17));
  list.append(BookNodeType("vendorCatalog", QMetaType::QString, 0, 0));
  list.append(BookNodeType("vendorListingid", QMetaType::QString, 0, 0));
  list.append(BookNodeType("volumeText", QMetaType::QString, 0, 0));
  list.append(BookNodeType("weight", QMetaType::Int, 0, 0));

  for (int i = 0; i < list.count(); i++) {
    BookNodeType bt = list.at(i);
    if (bt.name() == name())
      return bt;
  }

  return BookNodeType(QString("Unknown"), QMetaType::UnknownType, 0, 0);
}

QMetaType::Type BookListingNode::type() {
  return nodeData().type();
}

int BookListingNode::minLength() {
  return nodeData().minLength();
}

int BookListingNode::maxLength() {
  return nodeData().maxLength();
}

const QVariant BookListingNode::value() {
  QString buffer;
  if (hasChildNodes()) {
    QStringList items;
    QDomNodeList nodes = childNodes();
    for (int i = 0; i < nodes.count(); i++) {
      QDomNode n = nodes.at(i);
      if (n.nodeType() == QDomNode::TextNode) {
        items << n.nodeValue();
      } else if (n.nodeType() == QDomNode::ElementNode) {
        items << n.toElement().nodeValue();
      } else if (n.nodeType() == QDomNode::CDATASectionNode) {
        items << n.toCDATASection().nodeValue();
      }
    }
    buffer = items.join(" ");
  }

  if (type() == QMetaType::QString)
    return buffer;

  if (type() == QMetaType::Bool)
    return (buffer == "true");

  if (type() == QMetaType::Int) {
    bool b;
    int i = buffer.toInt(&b);
    if (b && i > 0)
      return i;
  }

  if (type() == QMetaType::LongLong) {
    bool b;
    qint64 d = buffer.toLongLong(&b);
    if (b && d > 0)
      return d;
  }

  if (type() == QMetaType::QDateTime) {
    QDateTime dt = QDateTime::fromString(buffer, HOMEBASE3_DATETIME_FORMAT);
    if (dt.isValid())
      return dt;
  }

  return QVariant(buffer);
}

const QJsonObject BookListingNode::toJson() {
  QJsonObject o;
  o.insert(name(), QJsonValue::fromVariant(value()));
  return o;
}
