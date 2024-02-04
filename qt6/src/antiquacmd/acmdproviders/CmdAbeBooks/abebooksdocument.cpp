// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksdocument.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QDomAttr>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomProcessingInstruction>
#include <QFile>
#include <QList>
#include <QLocale>
#include <QTextStream>
#include <QVariant>

AbeBooksDocument::AbeBooksDocument(const AbeBooksAccess &config,
                                   const QString &element)
    : QDomDocument{} {
  apiLogin = config.user;
  apiKey = config.key;
  const QDomProcessingInstruction _dpir = createProcessingInstruction(
      "xml", "version=\"1.0\" encoding=\"" + ABEBOOKS_CHARSET + "\"");
  appendChild(_dpir);

  QDomElement _actionNode = createElement(element);
  _actionNode.setAttribute("version", "1.1");
  appendChild(_actionNode);
}

AbeBooksDocument::AbeBooksDocument(const QDomDocument &other)
    : QDomDocument{other} {
  apiLogin = QString();
  apiKey = QString();
}

QPair<int, QString> AbeBooksDocument::errorResponseCode() {
  const QDomElement _root = documentElement();
  QPair<int, QString> _pair;
  if (notExists("requestError")) {
    _pair.first = 104;
    _pair.second = "Unknown XML Error";
    return _pair;
  }
  _pair.first = nodeIntValue(_root.namedItem("code"));
  _pair.second = _root.namedItem("message").firstChild().nodeValue();
  return _pair;
}

bool AbeBooksDocument::notExists(const QString &tag) const {
  return (documentElement().tagName() != tag);
}

const QDomNodeList AbeBooksDocument::fetchNodes(const QString &tag) const {
  return documentElement().elementsByTagName(tag);
}

const QDomNode AbeBooksDocument::firstChildNode(const QDomElement &parent,
                                                const QString &tag) const {
  if (parent.hasChildNodes()) {
    QDomNodeList _list = parent.childNodes();
    for (int i = 0; i < _list.count(); i++) {
      QDomNode _node = _list.at(i);
      if (_node.nodeName() == tag)
        return _node;
    }
  }
  return QDomNode();
}

qint64 AbeBooksDocument::nodeIntValue(const QDomNode &node) const {
  if (node.firstChild().isText()) {
    QVariant _value = node.firstChild().nodeValue();
    bool _b = false;
    qint64 _i = _value.toInt(&_b);
    if (_b)
      return _i;
  }
  return 0;
}

const QDomElement AbeBooksDocument::createAction(const QString &tag) {
  QDomElement _acNode = createElement("action");
  if (tag.isEmpty())
    _acNode.setAttribute("name", "getAllNewOrders");
  else
    _acNode.setAttribute("name", tag);

  QDomElement _username = createElement("username");
  _username.appendChild(createTextNode(apiLogin));
  _acNode.appendChild(_username);

  QDomElement _password = createElement("password");
  _password.appendChild(createTextNode(apiKey));
  _acNode.appendChild(_password);

  documentElement().appendChild(_acNode);
  return _acNode;
}

const QPair<QString, QString> AbeBooksDocument::getPurchaseMethod() {
  QPair<QString, QString> _pair;
  _pair.first = QString();
  _pair.second = QString();

  if (notExists("orderUpdateRequest"))
    return _pair;

  const QDomNodeList _list = fetchNodes("purchaseMethod");
  if (_list.count() < 1)
    return _pair;

  for (int i = 0; i < _list.count(); i++) {
    const QDomNode _node = _list.at(i);
    if (_node.hasAttributes()) {
      const QDomElement _element = _node.toElement();
      if (_element.hasAttribute("type")) {
        _pair.second = _element.attribute("type", "Invoice");
      }
    }
    if (_node.hasChildNodes() && _node.firstChild().isText()) {
      _pair.first = _node.firstChild().nodeValue();
    }
  }
  return _pair;
}

const QDomNodeList AbeBooksDocument::getPurchaseOrderList() {
  QDomNodeList _dummy;
  if (notExists("orderUpdateResponse"))
    return _dummy;

  if (fetchNodes("purchaseOrderList").count() != 1)
    return _dummy;

  QDomNodeList _list = documentElement().firstChild().childNodes();
  if (_list.count() < 1)
    return _dummy;

  int _count = 0;
  for (int i = 0; i < _list.count(); i++) {
    QDomNode n = _list.at(i);
    if (n.nodeName() != "purchaseOrder")
      continue;

    _count++;
  }
  return (_count >= 1) ? _list : _dummy;
}

const QTimeZone AbeBooksDocument::fetchTimeZone(const QDomElement &orderNode) {
  QDomNode _domain = firstChildNode(orderNode, "domain");
  QByteArray _iana;
  switch (_domain.toElement().attribute("id", "0").toInt()) {
  case 46: // abebooks.co.uk
    _iana = "UTC";
    break;

  case 47: // abebooks.de
    _iana = "Europe/Berlin";
    break;

  case 48: // abebooks.fr
    _iana = "Europe/Paris";
    break;

  case 51: // abebooks.it
    _iana = "Europe/Rome";
    break;

  default: // iberlibro.com zvab.com abebooks.com
    _iana = "America/Los_Angeles";
    break;
  };
  return QTimeZone(_iana);
}

const QDateTime AbeBooksDocument::getOrderDate(const QDomElement &orderNode) {
  QDomNode _odn = firstChildNode(orderNode, "orderDate");
  if (_odn.isNull())
    return QDateTime::currentDateTime();

  QDate _date = QDate::currentDate();
  const QDomElement _dn = _odn.namedItem("date").toElement();
  _date.setDate(nodeIntValue(_dn.namedItem("year")),
                nodeIntValue(_dn.namedItem("month")),
                nodeIntValue(_dn.namedItem("day")));

  QTime _time = QTime::currentTime();
  const QDomElement _tn = _odn.namedItem("time").toElement();
  _time.setHMS(nodeIntValue(_tn.namedItem("hour")),
               nodeIntValue(_tn.namedItem("minute")),
               nodeIntValue(_tn.namedItem("second")));

  QDateTime _datetime = QDateTime::currentDateTime();
  _datetime.setTimeZone(fetchTimeZone(orderNode));
  _datetime.setDate(_date);
  _datetime.setTime(_time);
  return _datetime.toLocalTime();
}

const QDomNodeList AbeBooksDocument::getOrderItemList(const QDomNode &node) {
  QDomNodeList _dummy;
  if (notExists("orderUpdateResponse"))
    return _dummy;

  // purchaseOrderNode
  QDomElement _pon = node.toElement();

  // purchaseOrderItemList
  QDomNodeList _list = _pon.elementsByTagName("purchaseOrderItemList");
  if (_list.size() != 1)
    return _dummy;

  // purchaseOrderItem
  return _list.at(0).toElement().elementsByTagName("purchaseOrderItem");
}

const QVariant AbeBooksDocument::getNodeValue(const QDomNode &parent) {
  return parent.firstChild().nodeValue();
}

const QString AbeBooksDocument::getTagText(const QDomNode &parent,
                                           const QString &tag) const {
  QString _string = parent.namedItem(tag).firstChild().nodeValue();
  return _string.trimmed();
}

const QVariant AbeBooksDocument::getAddressValue(const QDomNode &addressNode,
                                                 const QString &tag) {
  return addressNode.namedItem(tag).firstChild().nodeValue();
}

const QString AbeBooksDocument::getFullname(const QDomNode &addressNode) {
  QVariant _value = addressNode.namedItem("name").firstChild().nodeValue();
  QString _string = _value.toString().trimmed();
  _string.replace(AntiquaCRM::AUtil::spaceRegExp(), " ");
  return _string.trimmed();
}

const QPair<QString, QString>
AbeBooksDocument::getPerson(const QDomNode &addressNode) {
  QPair<QString, QString> _pair;
  QString _string = getFullname(addressNode);
  QStringList _list = _string.split(" ");
  _pair.second = _list.takeLast();
  _pair.first = _list.join(" ");
  return _pair;
}

const QVariant AbeBooksDocument::getPostalCode(const QDomNode &addressNode) {
  QString _plz = addressNode.namedItem("code").firstChild().nodeValue();
  _plz = _plz.trimmed();
  if (_plz.length() > 10 || _plz.indexOf(" ") > 6) {
    _plz = _plz.left(10).trimmed();
    QStringList _l = _plz.split(" ");
    if (_l.size() > 1) {
      _plz = _l.first().trimmed();
    }
    _l.clear();
  }
  return _plz;
}

const QString AbeBooksDocument::getLocation(const QDomNode &addressNode) {
  QVariant _value = addressNode.namedItem("city").firstChild().nodeValue();
  if (_value.isNull())
    return QString();

  return _value.toString().trimmed();
}

const QString AbeBooksDocument::getCountry(const QDomNode &addressNode) {
  QVariant _value = addressNode.namedItem("country").firstChild().nodeValue();
  if (_value.isNull())
    return QString();

  return _value.toString().trimmed();
}

const QString AbeBooksDocument::getStreet(const QDomNode &addressNode) {
  QStringList _list;
  foreach (QString _tag, QStringList({"region", "street", "street2"})) {
    QVariant _value = addressNode.namedItem(_tag).firstChild().nodeValue();
    QString _string = _value.toString().trimmed();
    if (_string.isEmpty())
      continue;

    /*
     Soll verhindern das Hausnummer/Postfach zweimal eingefügt wird!
     AbeBooks versteckt bei Kreditkartenzahlungen ein teil der Adresse und
     fügt diese dann Später hinzu. Was Eigentlich schwachsinn ist weil sie im
     Node::"street2" ohnehin enthalten ist. Außerdem ist hier kein System zu
     erkennen, manchmal ist sie Extra dann wieder nicht.
     Das heist - Es muß eine Abfrage gemacht werden ob dieser Teil im
     Node::"street" enhalten ist. Wenn nicht, dann von Node::"street2" diese
     wieder hinzufügen.
     NOTE - Sollte überwacht werden bis eine bessere Lösung gefunden ist!
            z.B. Mit einem Regulären Ausdruck das ganze verfeinern!
     */
    if (_tag == "street2" && _list.contains(_string))
      continue;

    _list << _string;
  }
  return _list.join(" ").trimmed();
}

const QString AbeBooksDocument::getPhone(const QDomNode &addressNode) {
  QVariant _value = addressNode.namedItem("phone").firstChild().nodeValue();
  QString _phone = _value.toString().trimmed();
  _phone.replace("+", "0");
  _phone.replace(AntiquaCRM::AUtil::nonDigitsRegExp(), "");
  return _phone;
}

const QString AbeBooksDocument::getEMail(const QDomNode &addressNode) {
  QVariant _value = addressNode.namedItem("email").firstChild().nodeValue();
  if (_value.isNull())
    return QString();

  QString _email = _value.toString().trimmed().toLower();
  return (_email.length() < 3) ? QString() : _email;
}
