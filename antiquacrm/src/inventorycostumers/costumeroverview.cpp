// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "costumeroverview.h"

#include <QtCore/QLocale>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

DomDocument::DomDocument(const QString &name) {
  QDomElement html = createElement("html");
  html.setAttribute("xmlns", "http://www.w3.org/1999/xhtml");
  html.setAttribute("xml:lang", QLocale().bcp47Name());
  appendChild(html);
  QDomElement head = createElement("head");
  html.appendChild(head);
  QDomElement meta = createElement("meta");
  meta.setAttribute("http-equiv", "Content-Type");
  meta.setAttribute("content", "text/html; charset=utf-8");
  head.appendChild(meta);
  QDomElement body = createElement("body");
  body.setAttribute("class", "body");
  html.appendChild(body);
  div = createElement("div");
  body.setAttribute("class", "container");
  body.appendChild(div);
}

QDomElement DomDocument::createElementNode(const QString &nodeName,
                                           const QString &data) {
  QDomElement element = createElement(nodeName);
  element.appendChild(createTextNode(data));
  return element;
}

QDomElement DomDocument::createLinkNode(const QString &set,
                                        const QString &data) {
  QDomElement element = createElement("p");
  QDomElement a = createElement("a");
  a.setAttribute("target", "_blank");
  if (set == "tel") {
    a.setAttribute("href", "tel:" + data);
    a.appendChild(createTextNode("Phone: " + data));
  } else if (set == "mail") {
    a.setAttribute("href", "mailto:" + data);
    a.appendChild(createTextNode("E-Mail: " + data));
  } else {
    a.setAttribute("href", data);
    a.appendChild(createTextNode(data));
  }
  element.appendChild(a);
  return element;
}

QDomElement DomDocument::createAddressNode(const QString &data) {
  QDomElement address = createElement("address");
  foreach (QString n, data.split("\n")) {
    QDomElement p = createElement("p");
    p.appendChild(createTextNode(n));
    address.appendChild(p);
  }
  return address;
}

Document::Document(QTextEdit *parent) : QTextDocument{parent} {
  setObjectName("AddressDocument");
  setMetaInformation(QTextDocument::DocumentTitle, tr("Costumer"));
  setDocumentMargin(10);
}

CostumerOverview::CostumerOverview(QWidget *parent) : QTextBrowser{parent} {
  setObjectName("CostumerOverview");
  setLineWrapMode(QTextEdit::WidgetWidth);
  setReadOnly(true);
  setOpenExternalLinks(true);
  setTextInteractionFlags(Qt::TextBrowserInteraction);

  document = new Document(this);
  QString css("div { page-break-before: always; }");
  css.append("body { padding: 10px;}");
  document->setDefaultStyleSheet(css);
  setDocument(document);
}

void CostumerOverview::createDocument(QHash<QString, QString> &data) {
  dom = new DomDocument("costumerview");
  QString buffer;
  if (data.contains("c_company_name")) {
    buffer = data.value("c_company_name");
    dom->div.appendChild(dom->createElementNode("h2", buffer));
  }

  QDomElement person = dom->createElement("div");
  dom->div.appendChild(person);
  if (data.contains("c_title")) {
    buffer = data.value("c_title");
    person.appendChild(dom->createTextNode(buffer+" "));
  }
  if (data.contains("fullname")) {
    buffer = data.value("fullname");
    person.appendChild(dom->createTextNode(buffer));
  }
  dom->div.appendChild(dom->createElement("hr"));

  dom->div.appendChild(dom->createElementNode("h4", tr("Address") + ":"));
  if (data.contains("c_postal_address")) {
    buffer = data.value("c_postal_address");
    dom->div.appendChild(dom->createAddressNode(buffer));
  }
  dom->div.appendChild(dom->createElement("hr"));

  QDomElement contact = dom->createElement("div");
  contact.appendChild(dom->createElementNode("h4", tr("Phone/E-Mail") + ":"));
  dom->div.appendChild(contact);
  if (data.contains("c_phone_0")) {
    buffer = data.value("c_phone_0");
    if (!buffer.isEmpty())
      contact.appendChild(dom->createLinkNode("tel", buffer));
  }
  if (data.contains("c_phone_1")) {
    buffer = data.value("c_phone_1");
    if (!buffer.isEmpty())
      contact.appendChild(dom->createLinkNode("tel", buffer));
  }
  if (data.contains("c_mobil_0")) {
    buffer = data.value("c_mobil_0");
    if (!buffer.isEmpty())
      contact.appendChild(dom->createLinkNode("tel", buffer));
  }
  if (data.contains("c_mobil_1")) {
    buffer = data.value("c_mobil_1");
    if (!buffer.isEmpty())
      contact.appendChild(dom->createLinkNode("tel", buffer));
  }
  if (data.contains("c_email_0")) {
    buffer = data.value("c_email_0");
    if (!buffer.isEmpty())
      contact.appendChild(dom->createLinkNode("mail", buffer));
  }
  if (data.contains("c_email_1")) {
    buffer = data.value("c_email_1");
    if (!buffer.isEmpty())
      contact.appendChild(dom->createLinkNode("mail", buffer));
  }
  dom->div.appendChild(dom->createElement("hr"));

  QDomElement additional = dom->createElementNode("h4", tr("Additional") + ":");
  dom->div.appendChild(additional);
  if (data.contains("c_since")) {
    QDateTime t = QDateTime::fromString(data.value("c_since"),Qt::ISODate);
    buffer = t.date().toString(Qt::RFC2822Date);
    buffer.prepend(": ");
    buffer.prepend(tr("Since"));
    additional.appendChild(dom->createElementNode("p", buffer));
  }

  document->setHtml(dom->toString(-1));
}
