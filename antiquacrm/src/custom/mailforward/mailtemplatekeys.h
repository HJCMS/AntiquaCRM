// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILTEMPLATEKEYS_UTILS_H
#define MAILTEMPLATEKEYS_UTILS_H

#include <QJsonObject>
#include <QObject>
#include <QMap>
#include <QVariant>

#include <SqlCore>

class MailTemplateKeys : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  QSqlQuery p_query;
  QMap<QString, QVariant> p_data;

  const QString completeName();
  const QString customerMail();
  const QString customerId();
  const QString padNumber(const QVariant &);

public:
  explicit MailTemplateKeys(QObject *parent = nullptr);
  bool setData(QMap<QString, QVariant> &);
  const QString getKey(const QString &key);
};

#endif // MAILTEMPLATEKEYS_UTILS_H
