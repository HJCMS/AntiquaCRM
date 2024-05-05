// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailtemplatekeys.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QMetaObject>
#include <QMetaType>

namespace AntiquaCRM {

MailTemplateKeys::MailTemplateKeys(QObject *parent) : QThread{parent} {}

const QString MailTemplateKeys::stringValue(const QString &key) {
  if (p_data.size() < 1)
    return QString();

  if (p_data.contains(key) &&
      p_data.value(key).metaType().id() == QMetaType::QString)
    return p_data.value(key).toString().trimmed();

  return QString();
}

const QString MailTemplateKeys::zerofilled(const QString &key) {
  if (p_data.size() < 1 || !p_data.contains(key))
    return QString();

  QString str = AntiquaCRM::AUtil::zerofill(p_data.value(key).toInt());
  return str.trimmed();
}

bool MailTemplateKeys::checkRequirements() {
  return (p_data.contains("c_firstname") && p_data.contains("c_lastname"));
}

const QString MailTemplateKeys::salutation() {
  if (p_data.value("c_gender").toInt() > 0) {
    int g = p_data.value("c_gender").toInt();
    if (g == 1) {
      // Männlich
      return tr("Dear Mr.");
    } else if (g == 2) {
      // Weiblich
      return tr("Dear Mrs.");
    }
  }
  return tr("Dear Ladies and Gentlemen");
}

const QString MailTemplateKeys::completeName() {
  QStringList list;
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

bool MailTemplateKeys::appendData(QMap<QString, QVariant> &data) {
  QMapIterator<QString, QVariant> it(data);
  while (it.hasNext()) {
    it.next();
    p_data.insert(it.key(), it.value());
  }

  if (p_data.size() > 0 && checkRequirements()) {
    p_data.insert("CRM_SALUTATION", salutation());
    p_data.insert("CRM_CUSTOMER_NAME", completeName());
    return true;
  }
  return false;
}

const QString MailTemplateKeys::articleList() {
  QStringList list;
  QJsonArray array = p_data.value("a_article_list").toJsonArray();
  if (array.size() > 0) {
    int l = 1;
    for (int i = 0; i < array.size(); i++) {
      QJsonObject obj = array[i].toObject();
      QString line(AntiquaCRM::AUtil::zerofill(l, 2) + ")");
      // Article
      int aid = obj.value("article").toInt();
      QString aid_str = AntiquaCRM::AUtil::zerofill(aid);
      line.append(" " + tr("Article") + ": " + aid_str);
      // Quantity
      line.append(" " + tr("Quantity") + ": ");
      line.append(QString::number(obj.value("count").toInt()));
      // Title
      line.append(" - " + obj.value("title").toString());
      list.append(line);
      l++;
    }
  }
  return list.join("\n");
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

  if (search == "CRM_SALUTATION")
    return salutation();

  if (search == "CRM_CUSTOMER_ID")
    return zerofilled("c_id");

  if (search == "CRM_ORDER_ID")
    return zerofilled("o_id");

  if (search == "CRM_PROVIDER_NAME")
    return stringValue("o_provider_name");

  if (search == "CRM_PROVIDER_ORDER_ID")
    return stringValue("o_provider_order_id");

  if (search == "CRM_INVOICE_ID")
    return zerofilled("o_invoice_id");

  if (search == "CRM_ARTICLE_ID")
    return stringValue("a_article_id");

  if (search == "CRM_ARTICLE_LIST")
    return articleList();

  if (search.contains("COMPANY_")) {
    return stringValue(search);
  }

#ifdef ANTIQUA_DEVELOPMENT
  qDebug() << "MailTemplateKeys:"
           << "Key „" << search << "“ not exists in „p_data“ Map!";
#endif

  return QString();
}

void MailTemplateKeys::run() {
  p_data.clear();
  m_sql = new AntiquaCRM::ASqlCore(this);
  AntiquaCRM::ASqlFiles file("query_company_data");
  if (file.openTemplate()) {
    QSqlQuery q = m_sql->query(file.getQueryContent());
    if (q.size() > 0) {
      while (q.next()) {
        p_data.insert(q.value("ac_class").toString(),
                      q.value("ac_value").toString());
      }
    }
    emit sendSuccess();
    return;
  }
  emit sendNoResult();
}

} // namespace AntiquaCRM
