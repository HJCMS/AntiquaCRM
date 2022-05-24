// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_BUCHFREUND_H
#define INVENTORY_PROVIDERS_BUCHFREUND_H

#include <QJsonDocument>
#include <QListWidget>
#include <QObject>
#include <QRegularExpression>
#include <QScrollArea>
#include <QWidget>

#include <Utils>
#include <AntiquaCRM>

namespace HJCMS {
class SqlCore;
};

class ProvidersOrderTable;
class ProvidersCustomerData;

/**
 * @class BF_Translater
 * @ingroup Providers
 * @brief Übersetzt die Buchfreund.de Json Datenfelder zu SQL Spaltenname.
 * Wir verwenden andere Datenfeld Bezeichnungen als der Dienstanbieter.
 * Deshalb müssen die Parameter vor der Weiterleitung an dieser Stelle übersetzt
 * werden.
 * @note Die Daten werden mit @ref ProviderOrders weitergeleitet!
 */
class BF_Translater : public QMap<QString, QString> {
public:
  explicit BF_Translater();

  /**
   * @brief SQL Datenfeld zurück geben
   * @param key - Json Parameter
   * @return SQL Column | QString()
   */
  const QString sqlParam(const QString &key);

  /**
   * @brief Such mit SQL Feldname nach Json Parameter
   * @param key - SQL Tabellen Feldname
   * @return Json Parameter | QString()
   */
  const QString jsonParam(const QString &key);
};

/**
 * @brief Stellt JSon Bestellanfragen
 * Es werden nur die Bestellungen der letzten
 * 14 Tage ausgegeben!
 */
class BF_JSonQuery : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Erstelle Url für die Abfrage
   * @param operation
   * @return
   */
  const QUrl apiQuery(const QString &operation);

Q_SIGNALS:
  void listResponsed(const QJsonDocument &doc);
  void orderResponsed(const QJsonDocument &doc);

public Q_SLOTS:
  void queryList();
  /**
   * @brief Detailabfrage mit BF-* Id stellen.
   * Api-JSon-Query BF::"bestellung"
   */
  void queryOrder(const QString &bfId);

public:
  explicit BF_JSonQuery(QObject * parent = nullptr);
};

class Buchfreund : public QScrollArea {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;

  ProvidersOrderTable *m_ordersTable;
  ProvidersCustomerData *m_customerData;

  /**
   * @brief Werte einfügen
   * @param objName
   * @param value
   */
  void setValue(const QString &objName, const QVariant &value);

private Q_SLOTS:
  void setContent(const QJsonDocument &);

public Q_SLOTS:
  QT_DEPRECATED void fetchOrderContent(const QString &bfid);

public:
  explicit Buchfreund(QWidget *parent = nullptr);
};

#endif // INVENTORY_PROVIDERS_BUCHFREUND_H
