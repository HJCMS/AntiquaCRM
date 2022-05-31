// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_WHSOFT_PLUGIN_H
#define ANTIQUA_WHSOFT_PLUGIN_H

#include <QContextMenuEvent>
#include <QLabel>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QVariant>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

/**
 * @brief Stellt JSon Bestellanfragen
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

public:
  explicit WHSoftJSonQuery(QObject *parent = nullptr);
};

class ANTIQUACORE_EXPORT WHSoftTable : public Antiqua::PurchaserOrderTable {
  Q_OBJECT

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void findArticleNumbers();

public:
  explicit WHSoftTable(QWidget *parent = nullptr);
};

class ANTIQUACORE_EXPORT WHSoftPurchaser final
    : public Antiqua::PurchaserWidget {
  Q_OBJECT

public Q_SLOTS:
  void setCustomerId(int customerId);
  void setValue(const QString &objName, const QVariant &value);

public:
  QTextEdit *rechnungsadresse;
  QTextEdit *lieferadresse;
  explicit WHSoftPurchaser(QWidget *parent = nullptr);
  const QVariant getValue(const QString &objName);
};

class ANTIQUACORE_EXPORT WHSoftWidget final : public Antiqua::InterfaceWidget {
  Q_OBJECT

private:
  WHSoftTable *m_orderTable;
  WHSoftPurchaser *m_purchaserWidget;
  QJsonDocument p_currentDocument;

  const QVariant tableData(int row, int column);

  /**
   * @brief Erstellt abfrage Datensatz für Kundenabfrage
   * @code
   *  {
   *    "provider" : String::"Antiqua::Interface::provider()",
   *    "type" : "customer_request",
   *    "c_firstname",  String::Vorname,
   *    "c_lastname",   String::Nachname,
   *    "c_postalcode", String::Postleitzahl,
   *    "c_location",   String::Wohnort
   *  }
   * @endcode
   * @see SIGNAL::checkCustomer(const QJsonDocument &)
   */
  const QJsonDocument customerRequest(const QJsonObject &object);

  /**
   * @brief Rechnungs und Lieferadressen einlesen
   */
  void parseAddressBody(const QString &section, const QJsonObject &object);

private Q_SLOTS:
  void readCurrentArticleIds();

public Q_SLOTS:
  /**
   * @brief createCustomerDocument
   */
  void createCustomerDocument();

  void setContent(const QJsonDocument &);

  /**
   * @brief Menü Einträge suchen
   */
  void createOrderRequest(const QString &bfId);

Q_SIGNALS:
  void s_checkArticles(QList<int> &);

public:
  WHSoftWidget(const QString &widgetId, QWidget *parent = nullptr);

  /**
   * @brief Kundennummer eintragen
   */
  void setCustomerId(int customerId);

  /**
   * @brief Übersetzt die Buchfreund.de Json Datenfelder zu SQL Spaltenname.
   * Wir verwenden andere Datenfeld Bezeichnungen als der Dienstanbieter.
   * Deshalb müssen die Parameter vor der Weiterleitung an dieser Stelle
   * übersetzt werden.
   */
  const QMap<QString, QString> fieldTranslate() const;

  /**
   * @brief Komplette Bestellung abfragen.
   */
  const ProviderOrder getProviderOrder();
};

class ANTIQUACORE_EXPORT WHSoft : public Antiqua::Interface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "de.hjcms.antiquacrm.AntiquaFactoryInterface" FILE "whsoft.json")
  Q_INTERFACES(Antiqua::Interface)

private:
  QObject *m_whsoft;
  WHSoftWidget *m_whsoftWidget;

private Q_SLOTS:
  void prepareJsonListResponse(const QJsonDocument &);

public:
  bool createInterface(QObject *parent);

  /**
   * @brief Anzeigefenster
   */
  Antiqua::InterfaceWidget *addWidget(const QString &widgetId, QWidget *parent);

  const QString provider() const;

  /**
   * @brief Wird für Anwender Konfiguration benötigt
   */
  inline const QString configGroup() const {
    return QString("provider/whsoft");
  };

  /**
   * @brief SQL Datenfeld zurück geben
   * @param key - Json Parameter
   * @return SQL Column | QString()
   */
  // const QString sqlParam(const QString &key);

  /**
   * @brief Such mit SQL Feldname nach API Parameter
   * @param key - SQL Tabellen Feldname
   * @return Json Parameter | QString()
   */
  // const QString apiParam(const QString &key);

  /**
   * @brief Menü Einträge suchen
   */
  void queryMenueEntries();
};

#endif // ANTIQUA_WHSOFT_PLUGIN_H
