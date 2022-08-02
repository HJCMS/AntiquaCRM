// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#ifndef ANTIQUA_INTERFACEWIDGET_H
#define ANTIQUA_INTERFACEWIDGET_H

#include <QFrame>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QObject>
#include <QPushButton>
#include <QScrollArea>
#include <QString>
#include <QTabWidget>
#include <QTextEdit>
#include <QToolBar>
#include <QWidget>

#include "antiqua_global.h"
#include "providerorders.h"

/**
 * @ingroup Antiqua Plugin Interface
 */
namespace Antiqua {

class PurchasePaymentInfo;
class PurchaserOrderTable;

#ifdef ANTIQUA_DEVELOPEMENT
class PurchaseDebugTable;
#endif

/**
 * @class Antiqua::InterfaceWidget
 * @ingroup Antiqua Plugin Interface
 * @brief Hier werden die Bestellungen verarbeitet.
 */
class ANTIQUACORE_EXPORT InterfaceWidget : public QScrollArea {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(QString providerName WRITE setProviderName READ getProviderName
                 NOTIFY sendProviderChanged)
  Q_PROPERTY(QString orderId WRITE setOrderId READ getOrderId NOTIFY
                 sendOrderIdChanged)
  Q_PROPERTY(int customerId WRITE setCustomerId READ getCustomerId NOTIFY
                 sendCustomerIdChanged)
  Q_PROPERTY(int orderExists WRITE setOrderExists READ getOrderExists NOTIFY
                 sendOrderExistsChanged)

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

  /**
   * @brief Aktuelle Kunden Nummer
   */
  int customerId = -1;

  /**
   * @brief Article Orders List
   * @see Antiqua::PurchaserOrderTable
   */
  Antiqua::PurchaserOrderTable *m_table;

  /**
   * @brief Kundendaten anzeigen und Artikel Prüfung
   */
  QToolBar *m_toolbar;

  /**
   * @brief Kundennummer Anzeigen/abfragen
   */
  QLineEdit *m_customerId;

  /**
   * @brief Full Purchaser Name
   */
  QLabel *m_customerInfo;

  /**
   * @brief Artikel Prüfung
   */
  QPushButton *btn_checkArticle;

  /**
   * @brief Tab Section
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
   * @brief Zahlungs Informationen
   */
  Antiqua::PurchasePaymentInfo *m_paymentInfo;

#ifdef ANTIQUA_DEVELOPEMENT
  /**
   * @brief Developement Summary TabelView
   */
  PurchaseDebugTable *m_debugTable;
#endif

  /**
   * @brief Felderdaten Puffer
   */
  QMap<QString, QVariant> m_purchaseData;

  /**
   * @brief Aktionsleiste für Bestellung bearbeiten!
   */
  QFrame *m_operationBar;

  /**
   * @brief Button für die Dienstleister Operationen
   */
  QPushButton *btn_providerActions;

protected:
  /**
   * @brief Ist für die Objekt-Initialisierung Notwendig.
   */
  const QString p_widgetId;

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
   * @see SIGNAL::sendCheckCustomer(const QJsonDocument &)
   */
  virtual const QJsonDocument customerRequest(const QJsonObject &object) = 0;

  /**
   * @brief Rechnungs und Lieferadressen einlesen
   */
  virtual void parseAddressBody(const QString &section,
                                const QJsonObject &object) = 0;

  /**
   * @brief Einfache Zeicheketten Bereinigung
   */
  const QString stripString(const QVariant &val) const;

  /**
   * @brief JSonValue Helfer
   * @ref stripString
   * Verwendet m_order->getValue(objName) und Konvertiert in das passende
   * Format.
   */
  const QJsonValue getString(const QString &objName);
  const QJsonValue getNumeric(const QString &objName);
  const QJsonValue getPrice(const QString &objName);

protected Q_SLOTS:
  /**
   * @brief Routine zum überprüfen der Kontaktdaten
   */
  virtual void checkCustomerExists() = 0;

  /**
   * @brief Routen zum überprüfen des Artikel bestandes
   */
  virtual void readCurrentArticleIds() = 0;

Q_SIGNALS:
  /**
   * @brief Wenn der Dienstleistername geändert wurde!
   */
  void sendProviderChanged(const QString &);

  /**
   * @brief Wenn der Beststellnummer geändert wurde!
   */
  void sendOrderIdChanged(const QString &);

  /**
   * @brief Wenn Kundennummer geändert wurde!
   */
  void sendCustomerIdChanged(int);

  /**
   * @brief Änderungen bei der Prüfung ob Bestellung existiert.
   */
  void sendOrderExistsChanged(int);

  /**
   * @brief Generieren eine Fehler Meldung
   */
  void sendErrorResponse(Antiqua::ErrorStatus type, const QString &message);

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
  void sendCheckCustomer(const QJsonDocument &);

  /**
   * @brief Kundendatensetz erstellen!
   */
  void sendCreateCustomer(const QJsonDocument &);

  /**
   * @brief Sende Signal mit Liste der eingefügten Bestellnummern
   */
  void sendCheckArticleIds(QList<int> &);

  /**
   * @brief Weiterleitung von Kontextmenü Tabellenansicht öffne Artikel.
   */
  void sendOpenArticle(int);

public Q_SLOTS:
  /**
   * @brief Erstelle Kundendatensatz für SIGNAL:sendCreateCustomer
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

  /**
   * @brief Methode zum Bearbeiten einer Dienstleister Bestellung
   * @note Hier werden die Plugin eigenen Operationen verabeitet!
   */
  virtual void createProviderOrderUpdate() = 0;

public:
  explicit InterfaceWidget(const QString &orderId, QWidget *parent = nullptr);

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
  void setCustomerId(int cId);

  /**
   * @brief Kundennummer abfragen!
   */
  int getCustomerId();

  /**
   * @brief Alle \b internen Artikel Nummern aus der Bestelltabelle ziehen.
   */
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
   * @brief Nehme Datensatz von Objekt
   */
  const QVariant getValue(const QString &objName);

  /**
   * @brief Für Telefon Nummern eine extra Eingabe.
   * Grund hierfür ist, das die DIenstleister diese nicht Validieren!
   */
  void setPhone(const QString &objName, const QVariant &value);

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

  /**
   * @brief Standard Suchfelder für Kundendaten ermittlung
   */
  inline const QStringList customerSearchFields() const {
    QStringList l({"c_firstname", "c_lastname", "c_postalcode", "c_location"});
    return l;
  }
};

};     // namespace Antiqua
#endif // ANTIQUA_INTERFACEWIDGET_H
