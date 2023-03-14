// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksdocument.h"

#include <QDebug>
#include <QDomAttr>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomProcessingInstruction>
#include <QFile>
#include <QList>
#include <QLocale>
#include <QRegExp>
#include <QTextStream>
#include <QVariant>

AbeBooksDocument::AbeBooksDocument(const AbeBooksAccess &config,
                                   const QString &element)
    : QDomDocument{} {
  apiLogin = config.user;
  apiKey = config.key;
  QDomProcessingInstruction pir = createProcessingInstruction(
      "xml", "version=\"1.0\" encoding=\"ISO-8859-1\"");
  appendChild(pir);

  QDomElement actionNode = createElement(element);
  actionNode.setAttribute("version", "1.1");
  appendChild(actionNode);
}

AbeBooksDocument::AbeBooksDocument(const QDomDocument &other)
    : QDomDocument{other} {
  apiLogin = QString();
  apiKey = QString();
}

QPair<int, QString> AbeBooksDocument::errorResponseCode() {
  QPair<int, QString> qp;
  if (notExists("requestError")) {
    qp.first = 104;
    qp.second = "Unknown XML Error";
    return qp;
  }
  qp.first = nodeIntValue(documentElement().namedItem("code"));
  qp.second = documentElement().namedItem("message").firstChild().nodeValue();
  return qp;
}

bool AbeBooksDocument::notExists(const QString &tag) const {
  return (documentElement().tagName() != tag);
}

const QDomNodeList AbeBooksDocument::fetchNodes(const QString &tag) const {
  return documentElement().elementsByTagName(tag);
}

const QDomNode AbeBooksDocument::firstChildNode(const QDomElement &parent,
                                                const QString &tag) const {
  QDomNode n;
  if (parent.hasChildNodes()) {
    QDomNodeList n_list = parent.childNodes();
    for (int i = 0; i < n_list.count(); i++) {
      QDomNode n = n_list.at(i);
      if (n.nodeName() == tag)
        return n;
    }
  }
  return n;
}

qint64 AbeBooksDocument::nodeIntValue(const QDomNode &node) const {
  if (node.firstChild().isText()) {
    QVariant val = node.firstChild().nodeValue();
    bool b = false;
    qint64 n = val.toInt(&b);
    if (b)
      return n;
  }
  return 0;
}

const QDomElement AbeBooksDocument::createAction(const QString &tag) {
  QDomElement actionNode = createElement("action");
  if (tag.isEmpty())
    actionNode.setAttribute("name", "getAllNewOrders");
  else
    actionNode.setAttribute("name", tag);

  QDomElement username = createElement("username");
  username.appendChild(createTextNode(apiLogin));
  actionNode.appendChild(username);

  QDomElement password = createElement("password");
  password.appendChild(createTextNode(apiKey));
  actionNode.appendChild(password);

  documentElement().appendChild(actionNode);
  return actionNode;
}

const QPair<QString, QString> AbeBooksDocument::getPurchaseMethod() {
  QPair<QString, QString> p;
  p.first = QString();
  p.second = QString();

  if (notExists("orderUpdateRequest"))
    return p;

  QDomNodeList list = fetchNodes("purchaseMethod");
  if (list.count() < 1)
    return p;

  for (int i = 0; i < list.count(); i++) {
    QDomNode n = list.at(i);
    if (n.hasAttributes()) {
      QDomElement e = n.toElement();
      if (e.hasAttribute("type")) {
        p.second = e.attribute("type", "Invoice");
      }
    }
    if (n.hasChildNodes() && n.firstChild().isText()) {
      p.first = n.firstChild().nodeValue();
    }
  }

  return p;
}

const QDomNodeList AbeBooksDocument::getPurchaseOrderList() {
  QDomNodeList empty;
  if (notExists("orderUpdateResponse"))
    return empty;

  if (fetchNodes("purchaseOrderList").count() != 1)
    return empty;

  QDomNodeList n_list = documentElement().firstChild().childNodes();
  if (n_list.count() < 1)
    return empty;

  int count = 0;
  for (int i = 0; i < n_list.count(); i++) {
    QDomNode n = n_list.at(i);
    if (n.nodeName() != "purchaseOrder")
      continue;

    count++;
  }
  return (count >= 1) ? n_list : empty;
}

const QTimeZone AbeBooksDocument::fetchTimeZone(const QDomElement &orderNode) {
  QDomNode domain = firstChildNode(orderNode, "domain");
  QByteArray iana;
  switch (domain.toElement().attribute("id", "0").toInt()) {
  case 46: // abebooks.co.uk
    iana = "UTC";
    break;

  case 47: // abebooks.de
    iana = "Europe/Berlin";
    break;

  case 48: // abebooks.fr
    iana = "Europe/Paris";
    break;

  case 51: // abebooks.it
    iana = "Europe/Rome";
    break;

  default: // iberlibro.com zvab.com abebooks.com
    iana = "America/Los_Angeles";
    break;
  };

  return QTimeZone(iana);
}

const QDateTime AbeBooksDocument::getOrderDate(const QDomElement &orderNode) {
  QDomNode odn = firstChildNode(orderNode, "orderDate");
  if (odn.isNull())
    return QDateTime::currentDateTime();

  QDate date = QDate::currentDate();
  QDomElement dn = odn.namedItem("date").toElement();
  date.setDate(nodeIntValue(dn.namedItem("year")),
               nodeIntValue(dn.namedItem("month")),
               nodeIntValue(dn.namedItem("day")));

  QTime time = QTime::currentTime();
  QDomElement tn = odn.namedItem("time").toElement();
  time.setHMS(nodeIntValue(tn.namedItem("hour")),
              nodeIntValue(tn.namedItem("minute")),
              nodeIntValue(tn.namedItem("second")));

  QDateTime dt = QDateTime::currentDateTime();
  dt.setTimeZone(fetchTimeZone(orderNode));
  dt.setDate(date);
  dt.setTime(time);
  return dt.toLocalTime();
}

const QDomNodeList AbeBooksDocument::getOrderItemList(const QDomNode &node) {
  QDomNodeList nodeList;
  if (notExists("orderUpdateResponse"))
    return nodeList;

  // purchaseOrderNode
  QDomElement pon = node.toElement();

  // purchaseOrderItemList
  QDomNodeList list = pon.elementsByTagName("purchaseOrderItemList");
  if (list.size() != 1)
    return nodeList;

  // purchaseOrderItem
  return list.at(0).toElement().elementsByTagName("purchaseOrderItem");
}

const QVariant AbeBooksDocument::getNodeValue(const QDomNode &parent) {
  QVariant out;
  out = parent.firstChild().nodeValue();
  return out;
}

const QString AbeBooksDocument::getTagText(const QDomNode &parent,
                                           const QString &tag) const {
  QString o = parent.namedItem(tag).firstChild().nodeValue();
  return o.trimmed();
}

const QVariant AbeBooksDocument::getAddressValue(const QDomNode &addressNode,
                                                 const QString &tag) {
  return addressNode.namedItem(tag).firstChild().nodeValue();
}

const QString AbeBooksDocument::getFullname(const QDomNode &addressNode) {
  QRegExp strip("[\\s\\t]+");
  QVariant v = addressNode.namedItem("name").firstChild().nodeValue();
  QString str = v.toString().trimmed().replace(strip, " ");
  return str.trimmed();
}

const QPair<QString, QString>
AbeBooksDocument::getPerson(const QDomNode &addressNode) {
  QPair<QString, QString> p;
  QString str = getFullname(addressNode);
  QStringList list = str.split(" ");
  p.second = list.takeLast();
  p.first = list.join(" ");
  return p;
}

const QVariant AbeBooksDocument::getPostalCode(const QDomNode &addressNode) {
  return addressNode.namedItem("code").firstChild().nodeValue();
}

const QString AbeBooksDocument::getLocation(const QDomNode &addressNode) {
  QVariant v = addressNode.namedItem("city").firstChild().nodeValue();
  return v.toString().trimmed();
}

const QString AbeBooksDocument::getCountry(const QDomNode &addressNode) {
  QVariant v = addressNode.namedItem("country").firstChild().nodeValue();
  if (v.isNull())
    return QString();

  return v.toString().trimmed();
}

const QString AbeBooksDocument::getStreet(const QDomNode &addressNode) {
  QStringList out;
  QStringList tags({"region", "street", "street2"});
  foreach (QString t, tags) {
    QVariant v = addressNode.namedItem(t).firstChild().nodeValue();
    QString s = v.toString().trimmed();
    if (s.isEmpty())
      continue;

    /**
     * @brief Soll verhindern das Hausnummer/Postfach zweimal eingefügt wird!
     * AbeBooks versteckt bei Kreditkartenzahlungen ein teil der Adresse und
     * fügt diese dann Später hinzu. Was Eigentlich schwachsinn ist weil sie im
     * Node::"street2" ohnehin enthalten ist. Außerdem ist hier kein System zu
     * erkennen, manchmal ist sie Extra dann wieder nicht. Das heist - Es muß
     * eine Abfrage gemacht werden ob dieser Teil im Node::"street" enhalten
     * ist. Wenn nicht, dann von Node::"street2" diese wieder hinzufügen.
     * @note Sollte überwacht werden bis eine bessere Lösung gefunden ist!
     * @todo Mit einem Regulären Ausdruck das ganze verfeinern!
     */
    if (t == "street2" && out.contains(s))
      continue;

    out << s;
  }
  return out.join(" ").trimmed();
}

const QString AbeBooksDocument::getPhone(const QDomNode &addressNode) {
  QVariant v = addressNode.namedItem("phone").firstChild().nodeValue();
  QString p = v.toString().trimmed();
  p.replace("+", "0");
  p.replace(QRegExp("\\D+"), "");
  return p;
}

const QString AbeBooksDocument::getEMail(const QDomNode &addressNode) {
  QVariant v = addressNode.namedItem("email").firstChild().nodeValue();
  if (v.isNull())
    return QString();

  QString email = v.toString().trimmed().toLower();
  return (email.isEmpty()) ? QString() : email;
}
