// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef WHSOFTJSONQUERY_PLUGIN_H
#define WHSOFTJSONQUERY_PLUGIN_H

#include <QJsonDocument>
#include <QString>
#include <QObject>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

/**
 * @brief Stellt JSon Bestellanfragen
 * @ingroup Whsoft Plugin
 * Es wird nur die Liste der letzten 14 Tage ausgegeben!
 */
class ANTIQUACORE_EXPORT WHSoftJSonQuery final : public QObject {
  Q_OBJECT

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

  /**
   * @brief Erweiterte abfragen
   * @param operation   - Aktion
   * @param doc         - Body
   */
  void customQuery(const QString &operation, const QJsonDocument &doc);

public:
  explicit WHSoftJSonQuery(QObject *parent = nullptr);
};

#endif // WHSOFTJSONQUERY_PLUGIN_H
