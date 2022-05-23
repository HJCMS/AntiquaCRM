// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_BUCHFREUND_H
#define INVENTORY_PROVIDERS_BUCHFREUND_H

#include <QJsonDocument>
#include <QListWidget>
#include <QObject>
#include <QRegularExpression>
#include <QSplitter>
#include <QWidget>

#include <Utils>

namespace HJCMS {
class SqlCore;
};

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
 * @class Buchfreundlist
 * @ingroup Providers
 * @brief Anzeige der Buchfreund Bestellliste
 */
class Buchfreundlist : public QListWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Erstellt ein ListWidgetItem und fügt es ein.
   */
  void addOrder(const QString &, const QDateTime &);

private Q_SLOTS:
  /**
   * @brief Signalverarbeitung für itemClicked
   */
  void getItemData(QListWidgetItem *);

public Q_SLOTS:
  /**
   * @brief Json Dokument einlesen
   */
  void setView(const QJsonDocument &doc);

Q_SIGNALS:
  /**
   * @brief Erstellt Json abfrage und leitet weiter
   */
  void orderClicked(const QJsonDocument &);

public:
  explicit Buchfreundlist(QWidget *parent = nullptr);
};

/**
 * @ingroup Providers
 * @class BuchfreundDisplay
 * @brief Anzeige der Buchfreund Bestelldetails
 */
class BuchfreundDisplay : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @section Bestellerdaten
   * @{
   */
  GenderBox *c_gender;
  LineEdit *c_firstname;
  LineEdit *c_lastname;
  LineEdit *c_location;
  LineEdit *c_street;
  EMailEdit *c_email_0;
  LineEdit *c_country;
  SerialID *c_id;
  PhoneEdit *c_phone_0;
  PostalCode *c_postalcode;

  /**
   * @}
   *
   * @section Bestell informationen
   * @{
   */
  SerialID *a_article_id;
  LineEdit *a_provider_id;
  IntSpinBox *a_count;
  PriceEdit *a_sell_price;
  QLineEdit *a_title;
  DateTimeEdit *a_modified;
  /** @} */

  QPushButton *btn_create_costumer;
  QPushButton *btn_search_costumer;
  QLabel *costumer_info;

  /**
   * @brief Datenbank Verbindung
   */
  HJCMS::SqlCore *m_sql;

  /**
   * @brief Wird für QObject::findchild benötigt!
   */
  const QRegularExpression p_objPattern =
      QRegularExpression("^[ac]_[a-z0-3_]+$");

  /**
   * @brief Mit @ref p_objPattern nach Datenfeldern suchen
   * @return QHash<ObjectName,Datenwert>
   */
  const QHash<QString, QVariant> createDataset();

private Q_SLOTS:
  /**
   * @brief SQL Kunde erstellen
   */
  void createSqlCostumer();

  /**
   * @brief SQL Kundenprüfung
   */
  void searchSqlCostumer();

  /**
   * @brief SQL Artikel Prüfung
   */
  void checkArticleId();

public Q_SLOTS:
  /**
   * @brief Durchlaufe das Json Dokument und ...
   * Übersetze mit @ref BF_Translater die Datenfelder,
   * bei erfolg werden sie an @ref setValue weiter gereicht.
   */
  void setContent(const QJsonDocument &);

  /**
   * @brief Befülle Datenfeld
   * @param objName  - Objektname des Datenfeldes
   * @param value    - Inhalt
   */
  void setValue(const QString &objName, const QVariant &value);

Q_SIGNALS:
  /**
   * @brief Fehler an Elternfenster senden!
   */
  void s_warning(const QString &);

  /**
   * @brief Wenn Kundennummer enthalten ...
   */
  void s_costumer(int);

public:
  explicit BuchfreundDisplay(QWidget *parent = nullptr);
};

class Buchfreund : public QSplitter {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Settings Konfigurations Gruppe
   */
  const QString configGroup;

  /**
   * @brief Erstelle Url für die Abfrage
   * @param operation
   * @return
   */
  const QUrl apiQuery(const QString &operation);

  /**
   * @brief Abfrage und erstellen der Listenansicht
   */
  void queryListViewContent();

public Q_SLOTS:
  /**
   * @brief Listenabfrage starten
   * Api-JSon-Query BF::"bestellungen"
   */
  void getOpenOrders();

  /**
   * @brief Detailabfrage mit BF-* Id stellen.
   * Api-JSon-Query BF::"bestellung"
   */
  void getOrderContent(const QJsonDocument &);

public:
  /**
   * @brief Liste der Offnenen Bestellungen
   */
  Buchfreundlist *bfList;

  /**
   * @brief Bestellungs Details
   */
  BuchfreundDisplay *bfDisplay;

  explicit Buchfreund(QWidget *parent = nullptr);
  // TEMP
  void testing();
};

#endif // INVENTORY_PROVIDERS_BUCHFREUND_H
