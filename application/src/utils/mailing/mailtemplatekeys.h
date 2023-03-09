// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILTEMPLATEKEYS_UTILS_H
#define MAILTEMPLATEKEYS_UTILS_H

#include <AntiquaCRM>
#include <QMap>
#include <QThread>
#include <QObject>
#include <QVariant>

/**
 * @ingroup utils
 * @brief eMail Template Keys class
 */
class MailTemplateKeys final : public QThread {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  QSqlQuery p_query;
  QMap<QString, QVariant> p_data;
  const QString stringValue(const QString &key);
  const QString zerofilled(const QString &key);
  bool checkRequirements();
  const QString salutation();
  const QString completeName();
  const QString customerMail();
  const QString customerId();
  const QString articleList();

public:
  explicit MailTemplateKeys(QObject *parent = nullptr);

  /**
   * @brief Datenpuffer speichern
   */
  bool setData(QMap<QString, QVariant> &);

  /**
   * @brief Datenfeld von QMap lesen
   * @param key - SQL Feldname
   */
  const QVariant getData(const QString &key);

  /**
   * @brief Template Konstanten Ersetzung
   */
  const QString convert(const QString &key);

  /**
   * @brief start request
   */
  void run() override;
};

#endif // MAILTEMPLATEKEYS_UTILS_H
