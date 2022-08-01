// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#ifndef ANTIQUA_INTERFACE_H
#define ANTIQUA_INTERFACE_H


#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMap>
#include <QObject>
#include <QString>
#include <QScrollArea>
#include <QWidget>

#include "antiqua_global.h"
#include "providerorders.h"

/**
 * @def ANTIQUA_INTERFACE
 * @ingroup Antiqua Plugin Interface
 */
#ifndef ANTIQUA_INTERFACE
#define ANTIQUA_INTERFACE "de.hjcms.@PROJECTNAME@.AntiquaFactoryInterface"
#endif

/**
 * @group Antiqua Plugin Interface
 */
namespace Antiqua {

  class PurchaseOverview;

  /**
   * @class Antiqua::InterfaceWidget
   * @ingroup Antiqua Plugin Interface
   * @brief Hier werden die Bestellungen verarbeitet.
   */
  class ANTIQUACORE_EXPORT InterfaceWidget : public QScrollArea
  {
    Q_OBJECT
    Q_PROPERTY(QString providerName WRITE setProviderName READ getProviderName NOTIFY providerChanged)
    Q_PROPERTY(QString orderId WRITE setOrderId READ getOrderId NOTIFY orderIdChanged)
    Q_PROPERTY(int orderExists WRITE setOrderExists READ getOrderExists NOTIFY orderExistsChanged)

  private:
    /**
     * @brief Bestehende Auftrags Nummer
     * Wenn 0 existiert keine Bestellung.
     */
    int orderExists = 0;

    /**
     * @brief Name des Dienstleisters
     */
    QString providerName;

    /**
     * @brief Auftragsnummer des Dienstleisters
     */
    QString orderId;

  protected:
    /**
     * @brief Ist für die Objekt-Initialisierung Notwendig.
     */
    const QString p_widgetId;

    /**
     * @brief Aktuelle Kunden Nummer
     */
    int currentCustomerId = -1;

    /**
     * @brief Bestellanzeige
     */
    Antiqua::PurchaseOverview *m_order;

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
    virtual const QJsonDocument customerRequest(const QJsonObject &object) = 0;

    /**
     * @brief Rechnungs und Lieferadressen einlesen
     */
    virtual void parseAddressBody(const QString &section, const QJsonObject &object) = 0;

    /**
     * @brief Einfache Zeicheketten Bereinigung
     */
    const QString stripString(const QVariant &val) const;

    /**
     * @brief JSonValue Helfer
     * @ref stripString
     * Verwendet m_order->getValue(objName) und Konvertiert in das passende Format.
     */
    const QJsonValue getString(const QString &objName);
    const QJsonValue getNumeric(const QString &objName);
    const QJsonValue getPrice(const QString &objName);

  protected Q_SLOTS:
    virtual void checkCustomerClicked() = 0;
    virtual void readCurrentArticleIds() = 0;
    /**
     * @brief Von setOrderUpdateTypes() definierten Updatestatus verarbeiten!
     */
    virtual void providerOrderUpdateStatus(Antiqua::PaymentStatus) = 0;


  Q_SIGNALS:
    /**
     * @brief Wenn der Dienstleistername gesetzt wurde!
     */
    void providerChanged();

    /**
     * @brief Wenn der Beststellnummer gesetzt wurde!
     */
    void orderIdChanged();

    /**
     * @brief Änderungen bei der Prüfung ob Bestellung existiert.
     */
    void orderExistsChanged();

    /**
     * @brief Generieren eine Fehler Meldung
     */
    void errorResponse(Antiqua::ErrorStatus type, const QString &message);

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

    /**
     * @brief Sende Signal mit Liste der eingefügten Bestellnummern
     */
    void checkArticleIds(QList<int> &);

    /**
     * @brief Weiterleitung von Kontextmenü Tabellenansicht öffne Artikel.
     */
    void openArticle(int);

  public Q_SLOTS:
    /**
     * @brief Erstelle Kundendatensatz für SIGNAL:createCustomer
     */
    virtual void createCustomerDocument() = 0;

    /**
     * @brief Abfragen für Anzeige verarbeiten ...
     */
    virtual void setContent(const QJsonDocument &) = 0;

    /**
     * @brief Datensatzabfrage
     */
    virtual void createOrderRequest() = 0;

  public:
    explicit InterfaceWidget(const QString &orderId, QWidget *parent = nullptr);

    /**
     * @brief Definitions/Übersetzung - Bestellung beim Dienstleister aktualisieren!
     * Diese Einstellung muss für jeden Dienstleister extra getroffen werden!
     * Die ComboBox Indexes sind Identisch mit "Antiqua::PaymentStatus"
     * @see Antiqua::PurchaseOverview::setOrderUpdateActions(const QMap<Antiqua::PaymentStatus, QString> &);
     * @li PaymentStatus - Index aus der ComboBox und gleich mit Antiqua::PaymentStatus
     * @li QString       - Parameter des Dienstleisters zuweisen
     */
    virtual void setOrderUpdateTypes() = 0;

    /**
     * @brief Dienstleister angaben setzen/lesen.
     */
    void setProviderName(const QString &name);
    const QString getProviderName();

    /**
     * @brief Bestellnumer setzen/lesen.
     */
    void setOrderId(const QString &id);
    const QString getOrderId();

    /**
     * @brief Id eines bestehenden Auftrags setzen/lesen.
     * Setzt "SQL::inventory_orders:o_id"
     */
    void setOrderExists(int oId);
    int getOrderExists();

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
     * @param attribute - Json Parameter
     * @return SQL Column | QString()
     */
    const QString sqlParam(const QString &attribute);

    /**
     * @brief Suche mit SQL Feldname nach API Parameter
     * @param value - SQL Tabellen Feldname
     * @return Json Parameter | QString()
     */
    const QString apiParam(const QString &value);

    /**
     * @brief Artikel Nummern aus der Bestelltabelle lesen!
     */
    const ProviderOrder getProviderOrder();
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
    Q_CLASSINFO("Version", "@ANTIQUACRM_VERSION_STRING@")
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
     *    "datum": ANTIQUA_DATETIME_FORMAT,
     *    "id": "Provider Bestellnummer"
     *  }]
     * }
     */
    virtual void prepareJsonListResponse(const QJsonDocument &) = 0;

  Q_SIGNALS:
    void s_errorResponse(Antiqua::ErrorStatus, const QString &);
    void s_queryResponse(bool successfull);
    void s_anErrorOccurred();

  public:
    virtual bool createInterface(QObject * parent) = 0;

    /**
     * @brief Tab das eingefügt wird wenn Daten empfangen werden!
     * Die einzelnen Tabs werden über den Objektnamen Identifiziert und sind deshalb Unique.
     * @note widgetId muss Identisch mit "objectName" und "tabTitle" sein!
     */
    virtual Antiqua::InterfaceWidget *addWidget(const QString &orderId, QWidget * parent) = 0;

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

    /**
     * @brief Bestellungen beantworten
     */
    virtual void updateOrderDelivery(const QJsonObject &jso) = 0;

    /**
     * @brief Artikelbestands Änderungen senden.
     */
    virtual void updateArticleCount(int articleId, int count) = 0;

    /**
     * @brief Bild für Artikelnummer hochladen.
     * @param articleId - Artikelnummer
     * @param data  - Base64 kodierte Zeichenkette
     */
    virtual void uploadArticleImage(int articleId, const QString &base64) = 0;
  };
};

Q_DECLARE_INTERFACE(Antiqua::Interface, ANTIQUA_INTERFACE)

#endif // ANTIQUA_INTERFACE_H
