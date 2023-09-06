// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksordermaininfo.h"

#include <QDebug>
#include <QDomNode>
#include <QLayout>
#include <QSizePolicy>
#include <QUrlQuery>

AbeBooksOrderMainInfo::AbeBooksOrderMainInfo(QWidget *parent)
    : QWidget{parent} {
  setWhatsThis(tr("Infopage for current AbeBooks order."));
  setContentsMargins(0, 0, 0, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 5, 0, 0);

  QLabel *m_lb = new QLabel(this);
  m_lb->setWordWrap(false);
  m_lb->setText(tr("Changes the Provider status of the current order."));
  m_lb->setStyleSheet("QLabel {font-weight:bold;}");
  layout->addWidget(m_lb);

  m_orderId = new QLabel(this);
  layout->addWidget(m_orderId);

  m_buyerId = new QLabel(this);
  layout->addWidget(m_buyerId);

  m_purchaseId = new QLabel(this);
  layout->addWidget(m_purchaseId);

  m_email = new QLabel(this);
  m_email->setTextFormat(Qt::RichText);
  m_email->setOpenExternalLinks(true);
  layout->addWidget(m_email);

  m_buyerInfo = new QLabel(this);
  m_buyerInfo->setTextFormat(Qt::RichText);
  m_buyerInfo->setOpenExternalLinks(true);
  layout->addWidget(m_buyerInfo);

  m_address = new QTextEdit(this);
  m_address->setStyleSheet("QTextEdit {border:none;background:transparent;}");
  m_address->setEnabled(false);
  layout->addWidget(m_address);

  layout->addStretch(1);
  setLayout(layout);
}

void AbeBooksOrderMainInfo::mailingAddress(const QDomNodeList &list) {
  QMap<QString, QString> _data;
  for (int i = 0; i < list.length(); i++) {
    QDomNode _n = list.at(i);
    if (_n.nodeType() != QDomNode::ElementNode)
      continue;

    _data.insert(_n.nodeName(), _n.toElement().text());
  }

  if (_data.size() < 1)
    return;

  m_address->clear();

  QString _address;
  _address.append(_data.value("name"));
  _address.append("\n");
  if (_data.value("street2").length() > 1) {
    _address.append(_data.value("street2"));
    _address.append("\n");
  }
  _address.append(_data.value("street"));
  _address.append("\n");

  _address.append(_data.value("code"));
  _address.append(" ");
  _address.append(_data.value("city"));
  _address.append("/");
  _address.append(_data.value("country"));
  _address.append("\n");

  if (_data.value("phone").length() > 5) {
    _address.append(_data.value("phone"));
  }

  m_address->setPlainText(_address);
  m_address->setEnabled(true);
}

void AbeBooksOrderMainInfo::setMail(const QString &mail) {
  QStringList _parts = mail.split("@");
  if (_parts.size() != 2)
    return;

  QUrl _url;
  _url.setScheme("mailto");
  _url.setUserName(_parts.first(), QUrl::StrictMode);
  _url.setHost(_parts.last(), QUrl::StrictMode);

  if (!_url.isValid())
    return;

  QString _html(tr("eMail") + ": ");
  _html.append("<a href='");
  _html.append(_url.toString().remove("/"));
  _html.append("'>");
  _html.append(mail);
  _html.append("</a>");

  m_email->setToolTip(mail);
  m_email->setText(_html);
}

void AbeBooksOrderMainInfo::setContentData(const QDomElement &element) {
  if (element.isNull())
    return;

  m_buyerId->setText(tr("Buyer Id: %1").arg(element.attribute("id", "0")));

  QDomNodeList _list = element.childNodes();
  for (int i = 0; i < _list.length(); i++) {
    QDomNode _n = _list.at(i);
    if (_n.nodeType() != QDomNode::ElementNode)
      continue;

    if (_n.nodeName().startsWith("email")) {
      QString _mail = _n.toElement().text().toLower();
      if (_mail.length() > 3)
        setMail(_mail);
    } else if (_n.nodeName().startsWith("mailingAddress")) {
      mailingAddress(_n.childNodes());
    }
  }
}

void AbeBooksOrderMainInfo::setOrderId(const QString &id) {
  m_orderId->setText(tr("Order Id: %1").arg(id));
}

void AbeBooksOrderMainInfo::setOrderPurchaseId(const QString &id) {
  m_purchaseId->setText(tr("Purchase Id: %1").arg(id));
}

void AbeBooksOrderMainInfo::setOrderLink(const QUrl &url) {
  QString _text =
      tr("Open AbeBooks <a href='%1'>order webpage</a>.").arg(url.toString());
  m_buyerInfo->setToolTip(tr("Open link to AbeBooks order actions page."));
  m_buyerInfo->setStatusTip(url.toDisplayString(QUrl::RemoveQuery));
  m_buyerInfo->setText(_text);
}
