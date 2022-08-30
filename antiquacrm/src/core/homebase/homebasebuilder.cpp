// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "homebasebuilder.h"
#include "booklistingnode.h"

#include <QDebug>
#include <QDir>
#include <QDomProcessingInstruction>
#include <QFileInfo>
#include <QTextStream>

HomeBaseBuilder::HomeBaseBuilder(const QString &name)
    : QDomDocument{}, // Default
      p_xmlns("http://www.abebooks.com/BookListingXml"),
      p_xmlns_xsi("http://www.w3.org/2001/XMLSchema-instance") {
  QString codec(HOMEBASE3_DEFAULT_CODEC);
  QDomProcessingInstruction pir = createProcessingInstruction(
      "xml", "version=\"1.0\" encoding=\"" + codec + "\"");
  appendChild(pir);
}

BookListing HomeBaseBuilder::createListingNode() {
  BookListing listing(createElement("BookListing"));
  return listing;
}

bool HomeBaseBuilder::initDocument() {
  rootNode = createElement("BookListingInventory");
  rootNode.setAttribute("xmlns", p_xmlns);
  rootNode.setAttribute("xmlns:xsi", p_xmlns_xsi);
  rootNode.setAttribute(
      "xsi:schemaLocation",
      "http://www.abebooks.com/BookListingXml "
      "http://www.abebooks.com/docs/Schemas/BookInventory.xsd");
  appendChild(rootNode);
  return true;
}

bool HomeBaseBuilder::createDocument() {
  if (!initDocument())
    return false;

  BookListing book = createListingNode();
  foreach (QString s, book.defaultNodeNames()) {
    BookListingNode bn = BookListingNode(createElement(s));
    if (bn.type() == QMetaType::QString) {
      bn.appendChild(createTextNode("TEXT"));
    }
    book.appendChild(bn);
  }
  rootNode.appendChild(book);

  return true;
}

bool HomeBaseBuilder::writeToFile(const QString &destDir) {
  QFileInfo info(destDir, "abebooks_catalog.xml");
  if (info.absoluteDir().isReadable()) {
    QFile fp(info.filePath());
    if (fp.open(QIODevice::WriteOnly)) {
      QTextStream out(&fp);
      out.setCodec(HOMEBASE3_DEFAULT_CODEC);
      out << toString(1);
      fp.close();
      qDebug() << "written to:" << fp.fileName();
    }
    return true;
  }
  return false;
}
