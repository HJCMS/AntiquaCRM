// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#ifndef ANTIQUA_INTERFACE_H
#define ANTIQUA_INTERFACE_H

#include <QContextMenuEvent>
#include <QDateTime>
#include <QGroupBox>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QListWidget>
#include <QMap>
#include <QMetaObject>
#include <QMetaType>
#include <QObject>
#include <QScrollArea>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QToolBar>
#include <QUrl>
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
 * @def ANTIQUA_DATETIME_FORMAT
 * @ingroup Antiqua Plugin Interface
 * @brief Das vom System vorgegebene Datumsformat ist ISO 8601!
 * Soll verhindern das bei der Umwandlung von einem String, die Konvertierung nicht fehlschlägt!
 * @code
 *  QJsonObject import;
 *  QJsonObject obj = jsonArray[i].toObject();
 *  QDateTime d = QDateTime::fromString(obj["datetime"].toString(), "yyyy-MM-dd hh:mm:ss");
 *  import.insert("datetime", QJsonValue(d.toString(ANTIQUA_DATETIME_FORMAT)));
 * @endcode
 */
#ifndef ANTIQUA_DATETIME_FORMAT
#define ANTIQUA_DATETIME_FORMAT Qt::ISODate
#endif

/**
 * @group Antiqua Plugin Interface
 */
namespace Antiqua {

  /**
   * @class Antiqua::PurchaserOrderTable
   * @ingroup Antiqua Plugin Interface
   * @brief Tabelle für die Bestellartikel anzeigen
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
  class ANTIQUACORE_EXPORT PurchaserOrderTable final : public QTableWidget {
    Q_OBJECT

  private Q_SLOTS:
    void copyIdToClipboard();

  protected:
    void addHeaderItem(int, const QString &name);
    void contextMenuEvent(QContextMenuEvent *);

  Q_SIGNALS:
    void findArticleNumbers();

  public:
    explicit PurchaserOrderTable(QWidget *parent = nullptr);
    QTableWidgetItem *createItem(const QString &title) const;
  };

  /**
   * @class Antiqua::ProviderWidget
   * @ingroup Antiqua Plugin Interface
   * @brief Haupseite des Dienstleisters
   */
  class ANTIQUACORE_EXPORT ProviderWidget : public QWidget
  {
    Q_OBJECT

  public:
    explicit ProviderWidget(const QString &widgetId, QWidget *parent = nullptr);

  };

  /**
   * @class Antiqua::PurchaseOverview
   * @ingroup Antiqua Plugin Interface
   * @brief Bestellübersicht
   */
  class ANTIQUACORE_EXPORT PurchaseOverview final : public QWidget
  {
    Q_OBJECT

  private:
    int c_id = 0;
    QToolBar *m_toolbar;
    QAction *ac_check;

    /**
     * @brief c_id
     */
    QLineEdit *m_customerId;

    /**
     * @brief Full Purchaser Name
     */
    QLineEdit *m_customerInfo;

    /**
     * @brief Tab Section
     * @{
     */
    QTabWidget *m_tabWidget;

    /**
     * @brief c_postal_address
     */
    QTextEdit *m_billingAddress;

    /**
     * @brief c_shipping_address
     */
    QTextEdit *m_shippingAddress;

    /**
     * @brief Käufer Anmerkungen und/oder Fragen
     */
    QTextEdit *m_comments;

    /**
     * @brief Customer Summary
     */
    QListWidget *m_summary;

    /**< @} */

    /**
     * @brief Article Orders List
     * @see Antiqua::PurchaserOrderTable
     */
    Antiqua::PurchaserOrderTable *m_table;

  public Q_SLOTS:
    /**
     * @brief Kundennummer einfügen
     */
    void setCustomerId(int id);

  Q_SIGNALS:
    void customerIdChanged(int);
    void checkOrders();

  public:
    explicit PurchaseOverview(const QString &id, QWidget *parent = nullptr);

    int getCustomerId();

    const QList<int> getArticleIDs();

    /**
     * @brief In Bestelltabelle eine reihe hinzufügen
     */
    void setTableCount(int count);

    /**
     * @brief Anzahl der Bestellartikel
     */
    int getTableCount();

    /**
     * @brief Eine Zelle in reihe Ändern/Einfügen
     */
    bool setTableData(int row, int column, const QVariant &value);

    /**
     * @brief Nehme Datensatz von Reihe/Zelle
     */
    const QVariant getTableData(int row, int column);

    /**
     * @brief Setze Datensatz auf Objektname
     */
    void setValue(const QString &objName, const QVariant &value);

    /**
     * @brief Für Telefon Nummern eine extra Eingabe.
     * Grund hierfür ist, das die DIenstleister diese nicht Validieren!
     */
    void setPhone(const QString &objName, const QVariant &value);

    /**
     * @brief Nehme Datensatz von Objekt
     */
    const QVariant getValue(const QString &objName);

    /**
     * @brief Käuferinformationen auslesen!
     */
    const QHash<QString,QVariant> getCustomerData();
  };

  /**
   * @class Antiqua::InterfaceWidget
   * @ingroup Antiqua Plugin Interface
   * @brief Hier werden die Bestellungen verarbeitet.
   */
  class ANTIQUACORE_EXPORT InterfaceWidget : public QScrollArea
  {
    Q_OBJECT

  protected:
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

  Q_SIGNALS:
    /**
     * @brief Generieren eine Fehler Meldung
     */
    void errorResponse(int type, const QString &message);

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
    void s_queryResponse(bool successfull);

  public:
    virtual bool createInterface(QObject * parent) = 0;

    /**
     * @brief Hauptseite des Dienstanbieters
     * Auf dieser Seite können zusätzliche Operationen zum Provider eingefügt werden.
     */
    virtual Antiqua::ProviderWidget *providerWidget(const QString &widgetId, QWidget * parent) = 0;

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

    /**
     * @brief Artikelbestands Änderungen senden.
     */
    virtual void updateArticleCount(int articleId, int count) = 0;
  };
};

Q_DECLARE_INTERFACE(Antiqua::Interface, ANTIQUA_INTERFACE)

#endif // ANTIQUA_INTERFACE_H
