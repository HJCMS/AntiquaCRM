// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_WHSOFT_PLUGIN_H
#define ANTIQUA_WHSOFT_PLUGIN_H

#include <QContextMenuEvent>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>
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

  /**
   * @brief Erweiterte abfragen
   * @param operation   - Aktion
   * @param doc         - Body
   */
  void customQuery(const QString &operation, const QJsonDocument &doc);

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

class ANTIQUACORE_EXPORT Buchfreund final : public Antiqua::ProviderWidget {
  Q_OBJECT

private:
  QLineEdit *m_articleId;
  QSpinBox *m_count;
  QTextEdit *m_response;

  void jsonQuery(const QString &operation, const QJsonDocument &doc);

private Q_SLOTS:
  void updateArticleCount();
  void queryResponse(const QJsonDocument &);

public:
  explicit Buchfreund(const QString &widgetId, QWidget *parent = nullptr);
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
  WHSoftWidget *m_whsoftWidget;   /**< Antiqua::InterfaceWidget */
  Buchfreund *m_buchfreundWidget; /**< Antiqua::ProviderWidget */

private Q_SLOTS:
  void prepareJsonListResponse(const QJsonDocument &);

public:
  bool createInterface(QObject *parent);

  /**
   * @brief Hauptseite
   */
  Antiqua::ProviderWidget *providerWidget(const QString &widgetId,
                                          QWidget *parent);

  /**
   * @brief Bestellungen
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
   * @brief Menü Einträge suchen
   */
  void queryMenueEntries();
};

#endif // ANTIQUA_WHSOFT_PLUGIN_H
