// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailtemplatekeys.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonValue>
#include <QMetaObject>

MailTemplateKeys::MailTemplateKeys(QObject *parent) : QThread{parent} {
  setObjectName("mail_templates");
}

const QString MailTemplateKeys::completeName() {
  QStringList list;
  if (p_data.value("c_gender").toInt() > 0) {
    int g = p_data.value("c_gender").toInt();
    if (g == 1) {
      list << tr("Mr");
    } else if (g == 2) {
      list << tr("Mrs");
    }
  }

  if (!p_data.value("c_title").toString().isEmpty())
    list << p_data.value("c_title").toString();

  if (!p_data.value("c_firstname").toString().isEmpty())
    list << p_data.value("c_firstname").toString();

  if (!p_data.value("c_lastname").toString().isEmpty())
    list << p_data.value("c_lastname").toString();

  return list.join(" ");
}

const QString MailTemplateKeys::customerMail() {
  if (!p_data.value("c_email_0").toString().isEmpty())
    return p_data.value("c_email_0").toString();

  if (!p_data.value("c_email_1").toString().isEmpty())
    return p_data.value("c_email_1").toString();

  return QString();
}

const QString MailTemplateKeys::customerId() {
  if (p_data.value("c_id").toInt() > 0)
    return QString::number(p_data.value("c_id").toInt());

  return QString();
}

const QString MailTemplateKeys::companySettings(const QString &key) {
  QString buffer;
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup("company");
  if (cfg.allKeys().contains(key)) {
    buffer = cfg.value(key).toString();
  }
  cfg.endGroup();
  return buffer;
}

const QString MailTemplateKeys::padNumber(const QVariant &v) {
  int i = v.toInt();
  if (i < 1)
    return QString();

  QString str = QString::number(i);
  return str.rightJustified(6, '0');
}

bool MailTemplateKeys::setData(QMap<QString, QVariant> &data) {
  p_data.clear();
  p_data = data;
  return (p_data.size() > 0);
}

const QVariant MailTemplateKeys::getData(const QString &key) {
  if (p_data.size() < 1)
    return QVariant();

  return p_data.value(key);
}

const QString MailTemplateKeys::convert(const QString &key) {
  QString search(key);
  search.replace("@", "");

  if (search.isEmpty())
    return QString();

  if (search == "CRM_CUSTOMER_NAME" || search == "PROVIDER_PURCHASER")
    return completeName();

  if (search == "CRM_CUSTOMER_EMAIL")
    return customerMail();

  if (search == "CRM_CUSTOMER_ID")
    return padNumber(p_data.value("c_id"));

  if (search == "CRM_ORDER_ID" || search == "CRM_PROVIDER_ORDER_ID")
    return padNumber(p_data.value("o_id"));

  if (search == "CRM_INVOICE_ID")
    return padNumber(p_data.value("o_invoice_id"));

  if (search == "CRM_ARTICLE_ID")
    return padNumber(p_data.value("a_article_id"));

  if (search == "COMPANY_SIGNATUR") {
    QString sig = companySettings("signature");
    sig.prepend("--\n");
    return sig;
  }

  return QString();
}

void MailTemplateKeys::run() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  p_query = m_sql->query("SELECT * FROM ui_template_keys;");
}
