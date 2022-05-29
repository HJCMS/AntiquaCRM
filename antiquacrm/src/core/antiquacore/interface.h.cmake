// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#ifndef ANTIQUA_INTERFACE_H
#define ANTIQUA_INTERFACE_H

#include <QGroupBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMap>
#include <QMetaObject>
#include <QMetaType>
#include <QObject>
#include <QScrollArea>
#include <QUrl>
#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "antiqua_global.h"
#include "providerorders.h"

#ifndef ANTIQUA_INTERFACE
# define ANTIQUA_INTERFACE "de.hjcms.@PROJECTNAME@.Antiqua"
#endif

/**
 * @group Antiqua Plugin Interface
 */
namespace Antiqua {

  /**
   * @class Antiqua::PurchaserOrderTable
   * @ingroup Antiqua Plugin Interface
   * @brief Tabelle für die Bestellartikel Anzeige
   * Die Standardformatierung der Zellen ist Text und alle Zellen sind
   * auf nur lesen gestellt. Die Tabelle dient Primär zur Bestellungsanzeige.
   * Es werden die Artikel Nr. mit Antiqua::InterfaceWidget::getArticleIds(column) ausgelesen.
   * Sie besteht aus 5 Spalten welche wie folgt vergeben sind:
   * @code
   *  --------------------------------------------------------
   *  |        0         |      1     |   2   |   3   |   4  |
   *  --------------------------------------------------------
   *  | Dienstleister ID | Artikel ID | Menge | Preis | Info |
   *  --------------------------------------------------------
   * @endcode
   */
  class ANTIQUACORE_EXPORT PurchaserOrderTable : public QTableWidget {
    Q_OBJECT

  protected:
    void addHeaderItem(int, const QString &name);

  public:
    explicit PurchaserOrderTable(QWidget *parent = nullptr);
    QTableWidgetItem *createItem(const QString &title) const;
  };

  /**
   * @class Antiqua::PurchaserWidget
   * @ingroup Antiqua Plugin Interface
   * @brief Käufer Informationen anzeigen ...
   */
  class ANTIQUACORE_EXPORT PurchaserWidget : public QGroupBox {
    Q_OBJECT

  protected:
    int id = 0;

  Q_SIGNALS:
    void customerIdChanged(int);

  public Q_SLOTS:
    virtual void setCustomerId(int customerId) = 0;
    virtual void setValue(const QString &objName, const QVariant &value) = 0;

  public:
    explicit PurchaserWidget(QWidget *parent = nullptr);
    int customerId();
    virtual const QVariant getValue(const QString &objName) = 0;
  };

  /**
   * @class Antiqua::InterfaceWidget
   * @ingroup Antiqua Plugin Interface
   */
  class ANTIQUACORE_EXPORT InterfaceWidget : public QScrollArea
  {
    Q_OBJECT

  protected:
    int currentCustomerId = -1;
    virtual const QVariant tableData(int row, int column) = 0;

  Q_SIGNALS:
    /**
     * @brief Kundenabfrage an das Hauptsystem
     * Es wird folgendes Format erwartet und jeder Parameter @b muss gesetzt sein!
     * @code
     *  {
     *    "provider" : String::"Antiqua::Interface::provider()",
     *    "providerid" : String:FensterId
     *    "type" : "customer_request",
     *    "c_firstname",  String::Vorname,
     *    "c_lastname",   String::Nachname,
     *    "c_postalcode", String::Postleitzahl,
     *    "c_location",   String::Wohnort
     *  }
     * @endcode
     */
    void checkCustomer(const QJsonDocument &);

    /**
     * @brief Kundendatensetz erstellen!
     */
    void createCustomer(const QJsonDocument &);

  public Q_SLOTS:
    /**
     * @brief Erstelle Kundendatensatz für SIGNAL:createCustomer
     *
     */
    virtual void createCustomerDocument() = 0;

    /**
     * @brief Abfragen für Anzeige verarbeiten ...
     */
    virtual void setContent(const QJsonDocument &) = 0;

    /**
     * @brief Datensatzabfrage
     */
    virtual void createOrderRequest(const QString &) = 0;

  public:
    explicit InterfaceWidget(const QString &widgetId, QWidget *parent = nullptr);

    /**
     * @brief Kundennummer setzen!
     * @note @ref currentCustomerId muss hier gesetzt werden!
     * @see getCustomerId()
     */
    virtual void setCustomerId(int customerId) = 0;

    /**
     * @brief Kundennummer abfragen!
     */
    int getCustomerId() { return currentCustomerId; };

    /**
     * @brief Übersetzt die Json Datenfelder zu SQL Spaltenname.
     * Wir verwenden andere Datenfeld Bezeichnungen als der Dienstanbieter.
     * Deshalb müssen die Parameter vor der Weiterleitung an dieser Stelle
     * übersetzt werden.
     */
    virtual const QMap<QString, QString> fieldTranslate() const = 0;

    /**
     * @brief SQL Datenfeld zurück geben
     * @param key - Json Parameter
     * @return SQL Column | QString()
     */
    const QString sqlParam(const QString &key);

    /**
     * @brief Suche mit SQL Feldname nach API Parameter
     * @param key - SQL Tabellen Feldname
     * @return Json Parameter | QString()
     */
    const QString apiParam(const QString &key);

    /**
     * @brief Artikel Nummer für Auftragserstellung zurück geben!
     */
    virtual const ProviderOrder getProviderOrder() = 0;
  };

  /**
   * @class Antiqua::Interface
   * @ingroup Antiqua Plugin Interface
   */
  class ANTIQUACORE_EXPORT Interface : public QObject
  {
    Q_OBJECT
    Q_CLASSINFO("Interface", ANTIQUA_INTERFACE)
    Q_CLASSINFO("Description", "@DISPLAYNAME@ Interface")
    Q_CLASSINFO("Version", "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@.@ANTIQUACRM_VERSION_RELEASE@")
    Q_CLASSINFO("Url", "https://www.hjcms.de")

  Q_SIGNALS:
    /**
     * @brief Wenn Bestellabfragen erfolgreich waren...
     * @see prepareJsonListResponse
     */
    void listResponse(const QJsonDocument &);

  protected Q_SLOTS:
    /**
     * @brief Anfrage in das vom System erwartete Format bringen.
     * {
     *  "provider":"String",
     *  "items":[{
     *    "datum": "String:Datum:ISO8601",
     *    "id": "Provider Bestellnummer"
     *  }]
     * }
     */
    virtual void prepareJsonListResponse(const QJsonDocument &) = 0;

  public:
    virtual bool createInterface(QObject * parent) = 0;

    /**
     * @brief Tab das eingefügt wird wenn Daten empfangen werden!
     * Die einzelnen Tabs werden über den Objektnamen Identifiziert und sind deshalb Unique.
     * @note widgetId muss Identisch mit "objectName" und "tabTitle" sein!
     */
    virtual Antiqua::InterfaceWidget *addWidget(const QString &widgetId, QWidget * parent) = 0;

    /**
     * @brief Setzt den Provider Schlüssel für die Identifizierung in Mehrdimensionalen abfragen!
     */
    virtual const QString provider() const = 0;

    /**
     * @brief Wird für Konfigurationen benötigt
     * Der Standard Prefix ist "provider/"
     */
    virtual const QString configGroup() const = 0;

    /**
     * @brief Abfrage ob Daten vorhanden sind.
     */
    virtual void queryMenueEntries() = 0;
  };
};

Q_DECLARE_INTERFACE(Antiqua::Interface, ANTIQUA_INTERFACE)

#endif // ANTIQUA_INTERFACE_H
