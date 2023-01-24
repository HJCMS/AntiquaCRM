// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDER_H
#define ANTIQUACRM_PROVIDER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QNetworkCookie>
#include <QObject>
#include <QUrl>

class SqlPsql;
class Settings;
class Networker;

/**
 * @brief Abstrakte Dienstleister Klasse
 * @section providers
 * Standardklasse für die Dienstleister-Bestellabfragen und Verarbeitung!
 */
class Provider : public QObject {
  Q_OBJECT

private:
  SqlPsql *m_sql;

protected:
  /**
   * @brief Ableitung von QSettings
   */
  Settings *m_config;

  /**
   * @brief Ableitung von QNetworkAccessManager
   */
  Networker *m_networker;

  /**
   * @brief Vordefinierte - Kekse für das Temporäre Zwischenspeichern
   */
  QNetworkCookie actionsCookie;
  QNetworkCookie authenticCookie = QNetworkCookie();

  /**
   * @brief Wie weit dürfen die Abfragen in der Vergangeheit liegen?
   * Dieser Parameter kann in der Konfiguration mit @i General/history_query
   * gelesen oder gesetzt werden. Weil der Server je nach Cron Konfiguration
   * alle paar Minuten abfragt. Serverausfälle maximal 1-2 Tage anhalten,
   * ist ein Wert von @i -3 völlig Ausreichend!
   * @note Ist für alle Dienstleister vorgesehen, der Minimalwert ist "-14"!
   *       Werte darunter werden von keinem Dienstleister unterstützt!
   * @warning Bei Werten unter \b -8 gibt Booklooker fehlerhafte Header zurück!
   * @default Der Standardwert ist -3 (Ein Wochenende)
   */
  qint8 history_query = -3;

  /**
   * @brief Standard abfrage URL zum Dienstleister
   * @list Beim bilden der Url ist folgendes zu beachten:
   *  @li Scheme und Host müssen gesetzt sein!
   *  @li Das Dienstleister Zertifikat muss im CA-Paket enthalten sein!
   * @note AntiquaCRM baut keine unsichere Verbindungen auf und es müssen
   * zusätzlich alle Cookies auf Secure gesetzt sein!
   * @code
   *  QUrl url;
   *  url.setSheme("https");
   *  url.setHost("hjcms.de");
   * @endcode
   */
  QUrl baseUrl;

  /**
   * @brief Optional - Benutzername
   */
  QString apiUser;

  /**
   * @brief Ein API Key ist zwingend erforderlich!
   * @see Settings "provider/{NAME}/api_key"
   */
  QString apiKey;

  /**
   * @brief Optional - URL Pfad
   */
  QString apiPath;

  /**
   * @brief Optional - Dienstanbieter Abfrageport
   * Bei "https" muss kein 443 angegeben werden!
   * Je nach Redirected ist die Angabe ein Nachteil!
   */
  qint64 apiPort;

  /**
   * @brief Dienstleistername Title in Groß-/Kleinschreibung!
   * Wird für Datenbank abfragen und der Darstellung im Menübaum verwendet!
   */
  virtual const QString provider() const = 0;

  /**
   * @brief Methode zum einlesen der Konfiguration
   * @note Sollte in @ref init() Eingebunden werden!
   */
  virtual void initConfiguration() = 0;

  /**
   * @brief Methode zum erstellen der Abfrage Url
   */
  virtual const QUrl apiQuery(const QString &) = 0;

  /**
   * @brief Hier wird das JSon-Dokument für den AntiquaCRM Import erstellt!
   * @see Documentation AntiquaCRM::JSON::Schema
   */
  virtual void prepareContent(const QJsonDocument &) = 0;

  /**
   * @brief Helfer - Korrigieren von fehlerhaften Namensangaben.
   */
  const QString ucFirst(const QString &str);

  /**
   * @brief Der \b VERSUCH - Die verschiedenen Dienstleisterangaben beim
   * Geschlecht, unter eine Hut zu bringen.
   */
  int convertGender(const QString &gender) const;

  /**
   * @brief IETF BCP 47 Sprach Attribute
   */
  const QString findBCP47(const QString &country) const;

  /**
   * @brief Suche Land mit IETF BCP 47 Sprachattribut
   */
  const QString getCountry(const QString &bcp47) const;

  /**
   * @brief Erstellt QDateTime aus QDate und QTime
   */
  const QDateTime getDateTime(const QString &dateString,
                              const QString &timeString,
                              Qt::TimeSpec spec = Qt::LocalTime) const;

  /**
   * @brief Konvertiert QDateTime in AntiquaCRM::ISODate Time
   */
  const QDateTime getDateTime(const QString &dateTimeString,
                              Qt::TimeSpec spec = Qt::LocalTime) const;

  /**
   * @brief Wird für die "Cookies" Zeitzonen benötigt.
   */
  const QDateTime timeSpecDate(const QDateTime &dateTime,
                               Qt::TimeSpec fromSpec = Qt::LocalTime) const;

  /**
   * @brief Helfer für dir Typenkonvertieren der Dienstleisterangaben!
   * @note Die Methode sollte beim Import immer verwendet werden!
   * @param key - Siehe Documentation AntiquaCRM::JSON::Schema
   */
  const QJsonValue convert(const QString &key, const QJsonValue &value) const;

  /**
   * @brief Liste der Dienstleister Bestellnummern
   * Gibt eine Liste der bereits Importierten Dienstleister Bestellnumern
   * zurück. Diese Methode @b MUSS immer vor @ref createOrders aufgerufen
   * werden! Andernfalls wird es beim SQL INSERT Fehler hageln!
   * @param provider - @b CaseSensitive
   */
  const QStringList currProviderIds(const QString &provider);

  /**
   * @brief Erstelle SQL-Inserts mit AntiquaCRM::JSON::Schema
   * @note Es werden nur SQL::INSERT's ausgeführt!
   */
  bool createOrders(const QList<QJsonObject> &orders);

  /**
   * @brief Kundensuche
   * @param AntiquaCRM::JSON::Schema "JsonObject::customer"
   * @note Um doppelte Ausgaben zu vermeiden muss "c_provider_import" immer
   * gesetzt sein - Siehe Documentation AntiquaCRM::JSON::Schema!
   * @return QPair<Kundennummer, "Kunden Vor- Nachname">
   */
  QPair<qint64, QString> findInsertCustomer(const QJsonObject &);

protected Q_SLOTS:
  /**
   * @brief Netzwerkantwort verarbeiten!
   */
  virtual void responsed(const QByteArray &) = 0;

Q_SIGNALS:
  /**
   * @brief Abfrage war Erfolglos oder wurde abgebrochen! :-(
   */
  void sendDisjointed();

  /**
   * @brief Abfrage war Erfolgreich :-)
   */
  void sendFinished();

public Q_SLOTS:
  /**
   * @brief Starte Dienstleister Prozess
   * @note Muss nach @ref init() aufgerufen werden!
   */
  virtual void start() = 0;

public:
  explicit Provider(QObject *parent = nullptr);

  /**
   * @brief Konfiguration laden und Parameter prüfen!
   * Wird vor @ref start() ausgeführt um Fehler zu vermeiden!
   */
  virtual bool init() = 0;
};

#endif // ANTIQUACRM_PROVIDER_H
