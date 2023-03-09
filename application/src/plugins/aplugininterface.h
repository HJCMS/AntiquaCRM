// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PLUGIN_INTERFACE_H
#define ANTIQUACRM_PLUGIN_INTERFACE_H

#include <AntiquaCRM>
#include <QDate>
#include <QDateTime>
#include <QDomDocument>
#include <QJsonDocument>
#include <QJsonValue>
#include <QNetworkCookie>
#include <QObject>
#include <QString>
#include <QTextCodec>
#include <QUrl>

namespace AntiquaCRM {

class UpdateDialog;

/**
 * @ingroup AntiquaCRMPlugin
 * @brief Klasse für die Dienstleisterkonfiuration
 */
struct ANTIQUACRM_LIBRARY APluginConfig {
  QString hostname = QString();
  QString querypath = QString();
  QString username = QString();
  QString apiKey = QString();
  int port = 443;
  int history = -7;
};

/**
 * @brief Primary Interface Class for Provider integration
 * @defgroup AntiquaCRMPlugin AntiquaCRM Plugin Interface
 *
 * @class APluginInterface
 * @brief AntiquaCRM Provider Plugin-Interface
 */
class ANTIQUACRM_LIBRARY APluginInterface : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Interface", ANTIQUACRM_INTERFACE)
  Q_CLASSINFO("Description", "AntiquaCRM Plugin Interface")
  Q_CLASSINFO("Version", ANTIQUACRM_VERSION)
  Q_CLASSINFO("Url", ANTIQUACRM_HOMEPAGE)

protected:
  /**
   * @brief Netzwerkabfragen
   */
  AntiquaCRM::ANetworker *m_network;

  /**
   * @brief Zeichensatz bei abweichungen
   * @note  AntiquaCRM verwendet UTF8
   */
  QTextCodec *m_decodeFrom;

  /**
   * @brief Url zur API Schnittstelle
   */
  QUrl apiUrl;

  /**
   * @brief API Benutzernamen
   */
  QString apiUser;

  /**
   * @brief API Schlüssel
   */
  QString apiKey;

  /**
   * @brief Sitzungs Keks
   * @note Kekse sollten in "Private Secure" initialisiert werden!
   */
  QNetworkCookie actionsCookie;

  /**
   * @brief Authentifizierungs Keks
   * @note Kekse sollten in "Private Secure" initialisiert werden!
   */
  QNetworkCookie authenticCookie = QNetworkCookie();

  /**
   * @brief Nehme Datum von ...
   * Dienstleister verwenden unterschiedliche Formatausgaben.
   * @note AntiquaCRM verwendet Standard ISO-Date
   */
  const QDateTime getDateTime(const QString &dateString,
                              const QString &timeString,
                              Qt::TimeSpec to = Qt::LocalTime) const;

  const QDateTime getDateTime(const QString &dateTimeString,
                              Qt::TimeSpec to = Qt::LocalTime) const;

  /**
   * @brief Wird benötigt für die Zeitzonen konvertierung bei den Keksen.
   */
  const QDateTime timeSpecDate(const QDateTime &dateTime,
                               Qt::TimeSpec to = Qt::LocalTime) const;

  /**
   * @brief Dienstleister geben Preise als Strings an.
   * @note AntiquaCRM verwendet double
   */
  double getPrice(const QJsonValue &value) const;

  /**
   * @brief Der Datumsimport ist je nach Dienstleister unterschiedlich.
   */
  virtual const QString
  dateString(const QDate &date = QDate::currentDate()) const = 0;

  /**
   * @brief Konfiguration einlesen
   */
  virtual bool initConfigurations() = 0;

  /**
   * @brief Erstellen der Abfrage Url.
   */
  virtual const QUrl apiQuery(const QString &section) = 0;

protected Q_SLOTS:
  /**
   * @brief Ist eine Authentifizierung notwendig?
   * Wenn ja kann diese hier Umgesetzt werden.
   */
  virtual void authenticate() = 0;

  /**
   * @brief Mancher Dienstanbieter verwendet einen anderen Zeichensatz!
   * AntiquaCRM verwendet UTF8, abweichungen sind auf Pluginseite zu behandeln!
   * @ref m_decodeFrom
   */
  void setContentDecoder(QTextCodec *);

  /**
   * @brief Vordefinierte funktionen für die Rückgabeverarbeitung.
   */
  virtual void prepareResponse(const QJsonDocument &js) = 0;
  virtual void prepareResponse(const QDomDocument &xml) = 0;

  /**
   * @brief Optional für die Verarbeitung von selbst Definierten abfragen
   */
  virtual void queryFinished(QNetworkReply *reply) = 0;

Q_SIGNALS:
  /**
   * @brief Nachrichten & Fehler Meldungen
   * Diese Signal kann dafür verwendet werden verschiedene Meldungen an das
   * Dienstleisterfenster zu senden.
   */
  void sendErrorResponse(AntiquaCRM::Message, const QString &);

  /**
   * @brief Vorgesehen für "Feedback" Nachrichten nach Updateaktionen.
   */
  void sendActionFinished(const QString &);

  /**
   * @brief Wird ausgelöst wenn die Verarbeitung der Anfrage abgeschlossen ist!
   * @note Dieses Signal muss immer nach Anfragebeendigung verwendet werden!
   */
  void sendQueryFinished();

  /**
   * @brief Wird ausgelöst wenn die Abfrage kein Ergebnis zurück gegeben hat.
   */
  void sendQueryAborted();

public Q_SLOTS:
  /**
   * @brief Info an das System ob ein Authentifizierung erforderlich ist!
   */
  virtual bool authenticationRequired() = 0;

  /**
   * @brief Eingang für Dienstleister Aktionen
   */
  virtual void orderUpdateAction(const QJsonObject &options) = 0;

public:
  explicit APluginInterface(QObject *parent = nullptr);

  /**
   * @brief Liest die Konfiguration des Dienstleisters ein.
   * @note Wird vom Konfigurations Dialog für die Zugangeinrichtung benötigt!
   * @ref AntiquaCRM::APluginConfig
   */
  const AntiquaCRM::APluginConfig getConfig(const QString &providerName);

  /**
   * @brief Dienstleister Dialog für Bestellstatus ändern!
   */
  virtual AntiquaCRM::UpdateDialog *updateDialog(QWidget *parent) = 0;

  /**
   * @brief Speicherschlüssel des Dienstleisters für die Konfiguration!
   * @note Muss in Kleinschreibung (ASCII), ohne Sonder-/Leerzeichen erfolgen!
   */
  virtual const QString configProvider() const = 0;

  /**
   * @brief Gibt den Anzeigenamen für das Interface zurück.
   * @note Für die Anzeige in Menüs, PopUp Fenster und Listen.
   */
  virtual const QString displayName() const = 0;

  /**
   * @brief Inititialisieren des Diesntleisterplugins.
   */
  virtual bool createInterface(QObject *parent) = 0;
};

}; // namespace AntiquaCRM

Q_DECLARE_METATYPE(AntiquaCRM::APluginConfig)
Q_DECLARE_INTERFACE(AntiquaCRM::APluginInterface, ANTIQUACRM_INTERFACE)

#endif // ANTIQUACRM_PLUGIN_INTERFACE_H
