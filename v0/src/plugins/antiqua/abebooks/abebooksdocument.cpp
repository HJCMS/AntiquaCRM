#include "abebooksdocument.h"
#include "abebooksconfig.h"

#include <QDomAttr>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomProcessingInstruction>
#include <QList>

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

const bool AbeBooksDocument::notExists(const QString &tag) const {
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

const qint64 AbeBooksDocument::nodeIntValue(const QDomNode &node) const {
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

const QDomElement AbeBooksDocument::getPurchaseOrder() {
  QDomElement empty;
  if (notExists("orderUpdateResponse"))
    return empty;

  return documentElement().namedItem("purchaseOrder").toElement();
}

const QDateTime AbeBooksDocument::getOrderDate(const QDomElement &parent) {
  QDateTime dt = QDateTime::currentDateTime();
  QDomNode n = firstChildNode(parent, "orderDate");
  if (!n.isNull()) {
    QDate date = QDate::currentDate();
    QDomElement d = n.namedItem("date").toElement();
    date.setDate(nodeIntValue(d.namedItem("year")),
                 nodeIntValue(d.namedItem("month")),
                 nodeIntValue(d.namedItem("day")));
    dt.setDate(date);

    QTime time = QTime::currentTime();
    QDomElement t = n.namedItem("time").toElement();
    time.setHMS(nodeIntValue(t.namedItem("hour")),
                nodeIntValue(t.namedItem("minute")),
                nodeIntValue(t.namedItem("second")));
    dt.setTime(time);
  }
  return dt;
}

const QDomElement AbeBooksDocument::getOrderItemList() {
  QDomElement out;
  if (notExists("orderUpdateResponse"))
    return out;

  QDomNodeList list = fetchNodes("purchaseOrderItemList");
  if (list.count() != 1)
    return out;

  return list.at(0).toElement();
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

const QVariant AbeBooksDocument::getAddressValue(const QString &tag) {
  QVariant out;
  QDomNodeList n_list = fetchNodes("mailingAddress");
  if (n_list.count() != 1)
    return out;

  return n_list.at(0).namedItem(tag).firstChild().nodeValue();
}
