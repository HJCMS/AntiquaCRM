// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_BUCHFREUND_H
#define INVENTORY_PROVIDERS_BUCHFREUND_H

#include <QJsonDocument>
#include <QList>
#include <QListWidget>
#include <QObject>
#include <QRegularExpression>
#include <QScrollArea>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Utils>

namespace HJCMS {
class SqlCore;
};

class ProvidersOrderTable;
class ProvidersCustomerData;

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
  explicit BF_JSonQuery(QObject *parent = nullptr);
};

class Buchfreund : public QScrollArea {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;

  Antiqua::Interface *m_bfPlugin;

  BF_JSonQuery *m_jsonQuery;

  ProvidersOrderTable *m_ordersTable;
  ProvidersCustomerData *m_customerData;

  void loadPlugins();

  /**
   * @brief Werte einfügen
   * @param objName
   * @param value
   */
  void setValue(const QString &objName, const QVariant &value);

public Q_SLOTS:
  void setContent(const QJsonDocument &);
  Q_INVOKABLE void checkCustomer();
  QT_DEPRECATED void testContent();

Q_SIGNALS:
  void openCustomer(int);

public:
  explicit Buchfreund(const QString &id, QWidget *parent = nullptr);
  const QList<int> getArticleIds();
  static void queryListEntries(QWidget *receiver);
};

#endif // INVENTORY_PROVIDERS_BUCHFREUND_H
