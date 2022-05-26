// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_WHSOFT_PLUGIN_H
#define ANTIQUA_WHSOFT_PLUGIN_H

#include <QObject>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

/**
 * @brief Stellt JSon Bestellanfragen
 * Es wird nur die Liste der letzten 14 Tage ausgegeben!
 */
class ANTIQUACORE_EXPORT WHSoftJSonQuery : public QObject {
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

public:
  explicit WHSoftJSonQuery(QObject *parent = nullptr);
};

class ANTIQUACORE_EXPORT WHSoftWidget : public Antiqua::InterfaceWidget {
  Q_OBJECT

public Q_SLOTS:
  void setContent(const QJsonDocument &);
  Q_INVOKABLE void checkCustomer();

public:
  WHSoftWidget(const QString &widgetId, QWidget *parent = nullptr);
  const QList<int> getArticleIds();
};

class ANTIQUACORE_EXPORT WHSoft : public Antiqua::Interface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "de.hjcms.antiquacrm.Antiqua.WHSoft" FILE "whsoft.json")
  Q_INTERFACES(Antiqua::Interface)

private:
  QObject *m_whsoft;

private Q_SLOTS:
  void prepareJsonListResponse(const QJsonDocument &);

public:
  bool createInterface(QObject *parent);

  Antiqua::InterfaceWidget *addWidget(const QString &widgetId, QWidget *parent);

  inline const QString configGroupName() const { return QString("whsoft"); };

  /**
   * @brief Übersetzt die Buchfreund.de Json Datenfelder zu SQL Spaltenname.
   * Wir verwenden andere Datenfeld Bezeichnungen als der Dienstanbieter.
   * Deshalb müssen die Parameter vor der Weiterleitung an dieser Stelle
   * übersetzt werden.
   */
  const QMap<QString, QString> fieldTranslate() const;

  /**
   * @brief SQL Datenfeld zurück geben
   * @param key - Json Parameter
   * @return SQL Column | QString()
   */
  const QString sqlParam(const QString &key);

  /**
   * @brief Such mit SQL Feldname nach API Parameter
   * @param key - SQL Tabellen Feldname
   * @return Json Parameter | QString()
   */
  const QString apiParam(const QString &key);

  /**
   * @brief Menü Einträge suchen
   */
  void queryMenueEntries();
};

#endif // ANTIQUA_WHSOFT_PLUGIN_H
