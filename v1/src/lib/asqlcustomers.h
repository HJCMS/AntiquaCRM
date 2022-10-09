// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SQLCUSTOMERS_H
#define ANTIQUACRM_SQLCUSTOMERS_H

#include <AGlobal>
#include <QHash>
#include <QMetaType>
#include <QObject>
#include <QRegExp>
#include <QSqlRecord>
#include <QStringList>

namespace AntiquaCRM {

/**
 * @brief Customers Class
 * @section Customers
 * This class is used to import and export Customers Data to different class
 * structures, it is mostly used by the Plugin Interface.
 * This is a final class, because it depends on the PgSQL Database.
 * @note It's a Container Class without Database read/write operations!
 */
class ANTIQUACRM_LIBRARAY ASqlCustomers final : public QObject {
  Q_OBJECT

private:
  const QSqlRecord p_record;

  /**
   * @brief temporary dataset
   */
  QHash<QString, QVariant> p_data;

public:
  explicit ASqlCustomers(QObject *parent = nullptr);

  /**
   * @brief prepare and normalize single strings
   */
  static const QString trim(const QString &str);

  /**
   * @brief Remove none Numbers from Mobile/Phone input
   */
  static const QString toPhone(const QString &phone);

  /**
   * @brief Regular Expression for eMail
   */
  static const QRegExp mailRegExp();

  /**
   * @brief Regular Expression for Phonenumbers
   * @note No url.scheme() supported!
   */
  static const QRegExp phoneRegExp();

  /**
   * @brief PgSQL:customers Table field list
   */
  const QStringList fieldNames() const;

  /**
   * @brief Represent required Datatype for "fieldname"
   */
  const QMetaType getType(const QString &fieldName) const;

  /**
   * @brief set Value for fieldname
   * @param fieldName
   * @param value
   */
  void setValue(const QString &fieldName, const QVariant &value);

  /**
   * @brief Read Value with key
   * @param fieldName
   * @return
   */
  const QVariant getValue(const QString &fieldName);

  /**
   * @brief given dataset
   */
  const QHash<QString, QVariant> getDataset();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLCUSTOMERS_H
