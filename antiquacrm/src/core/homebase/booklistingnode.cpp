// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklistingnode.h"
// #include "booknodetype.h"

#include <QDateTime>
#include <QDomText>
#include <QJsonValue>

BookListingNode::BookListingNode(const QDomElement &element)
    : QDomElement{element} {
}

const QString BookListingNode::name() { return QString(); }

QMetaType::Type BookListingNode::type() { return QMetaType::UnknownType; }

int BookListingNode::minLength() { return 0; }

int BookListingNode::maxLength() { return 30; }

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
