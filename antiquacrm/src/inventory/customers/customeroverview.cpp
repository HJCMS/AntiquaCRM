// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customeroverview.h"
#include "myicontheme.h"

#include <QDateTime>
#include <QDebug>
#include <QLocale>

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
  QString txt("div,p,br { page-break-before: always; }");
  QDomElement style = createElement("style");
  style.setAttribute("type", "text/css");
  style.appendChild(createTextNode(txt));
  head.appendChild(style);
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
  QDomElement element = createElement("dd");
  QDomElement p = createElement("p");
  QDomElement a = createElement("a");
  a.setAttribute("target", "_blank");
  if (set == "tel") {
    a.setAttribute("href", "tel:" + data);
  } else if (set == "mail") {
    a.setAttribute("href", "mailto:" + data);
  } else {
    a.setAttribute("href", data);
  }
  a.appendChild(createTextNode(data));
  p.appendChild(a);
  element.appendChild(p);
  return element;
}

QDomElement DomDocument::createAddressNode(const QString &data) {
  QDomElement address = createElement("dt");
  address.appendChild(createComment(" Address "));
  QRegExp reg("[\\n\\r]");
  foreach (QString n, data.split(reg)) {
    QDomElement p = createElement("dd");
    p.appendChild(createTextNode(n));
    address.appendChild(p);
  }
  return address;
}

CustomerOverview::CustomerOverview(QWidget *parent) : QTextBrowser{parent} {
  setObjectName("CustomerOverview");
  setLineWrapMode(QTextEdit::WidgetWidth);
  setReadOnly(true);
  setOpenExternalLinks(true);
  setTextInteractionFlags(Qt::TextBrowserInteraction);

  doc = new QTextDocument(this);
  doc->setDocumentMargin(25);
  doc->setIndentWidth(10);
  setDocument(doc);
}

bool CustomerOverview::check(const QString &key) {
  return (items.contains(key) && (!items.value(key).isEmpty()));
}

const QString CustomerOverview::convertDate(const QString &value) {
  QDateTime dt = QDateTime::fromString(value, Qt::ISODate);
  return QLocale::system().toString(dt, "dd MMMM yyyy");
}

void CustomerOverview::addLineBreak() {
  dom->div.appendChild(dom->createElement("br"));
}

void CustomerOverview::createCompanySection() {
  if (!check("c_company_name"))
    return;

  QString buffer;
  buffer = items.value("c_company_name");
  QDomElement m = dom->createElement("div");
  dom->div.appendChild(m);
  m.appendChild(dom->createElementNode("h2", buffer));
}

void CustomerOverview::createTitleSection() {
  QString buffer;
  QDomElement person = dom->createElement("div");
  dom->div.appendChild(person);
  if (check("c_locked") && (items.value("c_locked") == "true")) {
    QDomElement b = dom->createElement("h4");
    b.setAttribute("style", "color:red");
    b.appendChild(dom->createTextNode(tr("Locked")));
    person.appendChild(b);
  }
  if (check("c_title")) {
    buffer = items.value("c_title");
    person.appendChild(dom->createTextNode(buffer + " "));
  }
  if (check("fullname")) {
    buffer = items.value("fullname");
    person.appendChild(dom->createTextNode(buffer));
  }
  addLineBreak();
}

void CustomerOverview::createPhoneSection() {
  QString buffer;
  QDomElement phone = dom->createElement("div");
  dom->div.appendChild(phone);
  phone.appendChild(dom->createElementNode("dt", tr("Phone")));
  if (check("c_phone_0")) {
    buffer = items.value("c_phone_0");
    phone.appendChild(dom->createLinkNode("tel", buffer));
  }
  if (check("c_phone_1")) {
    buffer = items.value("c_phone_1");
    phone.appendChild(dom->createLinkNode("tel", buffer));
  }
  if (check("c_mobil_0")) {
    buffer = items.value("c_mobil_0");
    phone.appendChild(dom->createLinkNode("tel", buffer));
  }
  if (check("c_mobil_1")) {
    buffer = items.value("c_mobil_1");
    phone.appendChild(dom->createLinkNode("tel", buffer));
  }
}

void CustomerOverview::createEMailSection() {
  QString buffer;
  QDomElement email = dom->createElement("div");
  dom->div.appendChild(email);
  email.appendChild(dom->createElementNode("dt", tr("eMail")));
  if (check("c_email_0")) {
    buffer = items.value("c_email_0");
    email.appendChild(dom->createLinkNode("mail", buffer));
  }
  if (check("c_email_1")) {
    buffer = items.value("c_email_1");
    email.appendChild(dom->createLinkNode("mail", buffer));
  }
}

void CustomerOverview::createAddressSection() {
  if (!check("c_postal_address"))
    return;

  QDomElement address = dom->createElement("address");
  dom->div.appendChild(address);

  QString buffer = items.value("c_postal_address");
  address.appendChild(dom->createAddressNode(buffer));

  if (check("c_website")) {
    QDomElement p = dom->createElement("p");
    address.appendChild(p);
    buffer = items.value("c_website");
    p.appendChild(dom->createLinkNode("", buffer));
  } else {
    addLineBreak();
  }
}

void CustomerOverview::createAdditionalSection() {

  QDomElement additional =
      dom->createElementNode("dt", tr("Additional") + ": ");
  dom->div.appendChild(additional);

  if (check("c_transactions")) {
    QString buffer = items.value("c_transactions");
    buffer.prepend(tr("Transactions") + ": ");
    additional.appendChild(dom->createElementNode("dd", buffer));
    addLineBreak();
  }

  if (check("c_purchases")) {
    QString buffer = items.value("c_purchases");
    buffer.prepend(tr("Purchases") + ": ");
    additional.appendChild(dom->createElementNode("dd", buffer));
    addLineBreak();
  }

  if (check("c_changed")) {
    QString buffer = convertDate(items.value("c_changed"));
    buffer.prepend(tr("last Modified") + ": ");
    additional.appendChild(dom->createElementNode("dd", buffer));
    addLineBreak();
  }

  if (check("c_since")) {
    QString buffer = convertDate(items.value("c_since"));
    buffer.prepend(tr("Since") + ": ");
    additional.appendChild(dom->createElementNode("dd", buffer));
    addLineBreak();
  }
}

void CustomerOverview::createDocument(const DataFields &data) {
  for (int i = 0; i < data.count(); i++) {
    DataField df = data.at(i);
    // qDebug() << Q_FUNC_INFO << df.field() << df.value();
    items.insert(df.field(), df.value().toString());
  }
  dom = new DomDocument("customerview");
  createCompanySection();
  createTitleSection();
  createAddressSection();
  createPhoneSection();
  createEMailSection();
  createAdditionalSection();
  QString xhtml(dom->toString(-1));
  // qDebug() << Q_FUNC_INFO << xhtml;
  doc->setHtml(xhtml);
  doc->setModified(true);
  items.clear();
}
