// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailtemplatekeys_p.h"

#ifdef ANTIQUA_DEVELOPMENT
#  include <QDebug>
#endif
#include <QJsonArray>
#include <QJsonValue>
#include <QMetaType>
#include <QSqlQuery>

namespace AntiquaCRM
{

MailTemplateKeys::MailTemplateKeys(QObject* parent) : QThread{parent} {
}

const QString MailTemplateKeys::stringValue(const QString& key) {
  if (p_data.size() < 1)
    return QString();

  if (p_data.contains(key) && p_data.value(key).metaType().id() == QMetaType::QString)
    return p_data.value(key).toString().trimmed();

  return QString();
}

const QString MailTemplateKeys::zerofilled(const QString& key) {
  if (p_data.size() < 1 || !p_data.contains(key))
    return QString();

  QString _str = AntiquaCRM::AUtil::zerofill(p_data.value(key).toInt());
  return _str.trimmed();
}

bool MailTemplateKeys::checkRequirements() {
  return (p_data.contains("c_firstname") && p_data.contains("c_lastname"));
}

const QString MailTemplateKeys::salutation() {
  if (p_data.value("c_gender").toInt() > 0) {
    int _g = p_data.value("c_gender").toInt();
    // Männlich
    if (_g == 1)
      return tr("Dear Mr.");

    // Weiblich
    if (_g == 2)
      return tr("Dear Mrs.");
  }
  return tr("Dear Ladies and Gentlemen");
}

const QString MailTemplateKeys::completeName() {
  QStringList _l;
  if (!p_data.value("c_title").toString().isEmpty())
    _l << p_data.value("c_title").toString();

  if (!p_data.value("c_firstname").toString().isEmpty())
    _l << p_data.value("c_firstname").toString();

  if (!p_data.value("c_lastname").toString().isEmpty())
    _l << p_data.value("c_lastname").toString();

  return _l.join(" ");
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

bool MailTemplateKeys::appendData(QMap<QString, QVariant>& data) {
  QMapIterator<QString, QVariant> _it(data);
  while (_it.hasNext()) {
    _it.next();
    p_data.insert(_it.key(), _it.value());
  }

  if (p_data.size() > 0 && checkRequirements()) {
    p_data.insert("CRM_SALUTATION", salutation());
    p_data.insert("CRM_CUSTOMER_NAME", completeName());
    return true;
  }
  return false;
}

const QString MailTemplateKeys::articleList() {
  QStringList _list;
  QJsonArray _array = p_data.value("a_article_list").toJsonArray();
  if (_array.size() > 0) {
    int l = 1;
    for (int i = 0; i < _array.size(); i++) {
      QJsonObject _obj = _array[i].toObject();
      QString _line(AntiquaCRM::AUtil::zerofill(l, 2) + ")");
      // Article
      int aid = _obj.value("article").toInt();
      QString aid_str = AntiquaCRM::AUtil::zerofill(aid);
      _line.append(" " + tr("Article") + ": " + aid_str);
      // Quantity
      _line.append(" " + tr("Quantity") + ": ");
      _line.append(QString::number(_obj.value("count").toInt()));
      // Title
      _line.append(" - " + _obj.value("title").toString());
      _list.append(_line);
      l++;
    }
  }
  return _list.join("\n");
}

bool MailTemplateKeys::matchKeys(const QStringList& keys, const QString& data) const {
  if (keys.size() < 1)
    return false;

  return (keys.contains(data, Qt::CaseSensitive));
}

const QVariant MailTemplateKeys::getData(const QString& key) {
  if (p_data.size() < 1)
    return QVariant();

  return p_data.value(key);
}

const QString MailTemplateKeys::convert(const QString& key) {
  const QString _key = key.trimmed().replace("@", "");
  QString _buffer;
  if (_key.isEmpty())
    return _buffer;

  if (matchKeys(QStringList({"CRM_CUSTOMER_NAME", "PROVIDER_PURCHASER"}), _key))
    return completeName();

  if (matchKeys(QStringList({"CRM_CUSTOMER_EMAIL"}), _key))
    return customerMail();

  if (matchKeys(QStringList({"CRM_SALUTATION"}), _key))
    return salutation();

  if (matchKeys(QStringList({"CRM_CUSTOMER_ID"}), _key))
    return zerofilled("c_id");

  if (matchKeys(QStringList({"CRM_ORDER_ID"}), _key))
    return zerofilled("o_id");

  if (matchKeys(QStringList({"CRM_PROVIDER_NAME"}), _key))
    return stringValue("o_provider_name");

  if (matchKeys(QStringList({"CRM_PROVIDER_ORDER_ID"}), _key))
    return stringValue("o_provider_order_id");

  if (matchKeys(QStringList({"CRM_INVOICE_ID"}), _key))
    return zerofilled("o_invoice_id");

  if (matchKeys(QStringList({"CRM_ARTICLE_ID"}), _key))
    return stringValue("a_article_id");

  if (matchKeys(QStringList({"CRM_ARTICLE_LIST"}), _key))
    return articleList();

  if (_key.contains("COMPANY_", Qt::CaseSensitive)) {
    return stringValue(_key);
  }

#ifdef ANTIQUA_DEVELOPMENT
  qDebug() << "MailTemplateKeys:"
           << "Key „" << _key << "“ not exists in „p_data“ Map!";
#endif

  return _buffer;
}

void MailTemplateKeys::run() {
  p_data.clear();
  m_sql = new AntiquaCRM::ASqlCore(this);
  AntiquaCRM::ASqlFiles _file("query_company_data");
  if (_file.openTemplate()) {
    QSqlQuery _query = m_sql->query(_file.getQueryContent());
    if (_query.size() > 0) {
      while (_query.next()) {
        p_data.insert(_query.value("ac_class").toString(), _query.value("ac_value").toString());
      }
    }
    if (p_data.size() > 0) {
      emit sendSuccess();
      return; // ok step out
    }
  }
  emit sendNoResult();
}

} // namespace AntiquaCRM
