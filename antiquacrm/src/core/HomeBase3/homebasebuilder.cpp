// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "homebasebuilder.h"

#include <QDomProcessingInstruction>

HomeBaseBuilder::HomeBaseBuilder() : QDomDocument{} {
  QDomProcessingInstruction pir = createProcessingInstruction(
      "xml", "version=\"1.0\" encoding=\"ISO-8859-1\"");
  appendChild(pir);

  rootNode = createElement("BookListingInventory");
  rootNode.setAttribute("xmlns", "http://www.abebooks.com/BookListingXml");
  rootNode.setAttribute("xmlns:xsi",
                        "http://www.w3.org/2001/XMLSchema-instance");
  rootNode.setAttribute(
      "xsi:schemaLocation",
      "http://www.abebooks.com/BookListingXml "
      "http://www.abebooks.com/docs/Schemas/BookInventory.xsd");
  appendChild(rootNode);
}
