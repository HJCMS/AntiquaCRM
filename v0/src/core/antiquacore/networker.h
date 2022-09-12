// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef NETWORKER_NETWORKING_H
#define NETWORKER_NETWORKING_H

#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QSslConfiguration>
#include <QSslError>
#include <QTextCodec>

#include <AntiquaCRM>

namespace Antiqua {
  /**
   * @group Networking
   * @brief Standard Klasse für Netzwerkabfragen.
   * @class Networker
   *
   * Hier werden Sprache, Zeichensatz und Fehlermeldungen verabeitet.
   * Die Netzwerk antworten müssen in den Abfrage-Klassen verarbeitet werden!
   */
  class ANTIQUACORE_EXPORT Networker final : public QNetworkAccessManager {
    Q_OBJECT
    Q_CLASSINFO("Author", "Jürgen Heinemann")
    Q_CLASSINFO("URL", "https://www.hjcms.de")

  private:
    int tranfer_timeout = 15;
    /**
     * @brief Einstellungen lesen
     */
    ApplSettings *config;

    /**
     * @brief TextCodec für Read/Write Operationen
     */
    QTextCodec *m_codec;

    /**
     * @brief Standard Zeichensatz der verwendet wird.
     */
    QString p_charset = QString("utf8");

    /**
     * @brief Registriere Reply für die Fehlerbehandlung
     * @see jsonPostRequest und jsonGetRequest
     */
    QNetworkReply *reply;

    /**
     * @brief Setze CA-Bundle und SSL-Protokoll
     */
    const QSslConfiguration sslConfigguration();

    /**
     * @brief HTTP-Accept-Language (RFC2616)
     */
    const QByteArray languageRange();

    /**
     * @brief HTTP-Accept (RFC2616) Json
     */
    const QByteArray acceptJson();

    /**
     * @brief HTTP-Header Content-Type Json deklaration.
     */
    const QByteArray headerJson();

  private Q_SLOTS:
    /**
     * @brief Wenn die Anfrage beendet wurde.
     */
    void slotFinished(QNetworkReply *reply);

  public Q_SLOTS:
    /**
     * @brief Übertragungsfehler verarbeiten
     */
    void slotError(QNetworkReply::NetworkError error);

    /**
     * @brief SSL Fehlerverarbeitung
     */
    void slotSslErrors(const QList<QSslError> &list);

  Q_SIGNALS:
    /**
     * @brief Anfrage abgeschlossen
     * @param errors - Wenn ja true
     */
    void requestFinished(bool errors);

  public:
    explicit Networker(QObject *parent = nullptr);

    /**
     * @brief Erstelle eine Json HTTP_POST Anfrage
     * @note Wenn es eine https Verbindung ist und der
     *    Host mit QUrl:setHost() in der URL gesetzt ist.
     *    Wird SSL Verschlüsselung verwendet (sonst nicht)!
     * @param url   Anfrage URL
     * @param name  HTTP_FORM_DATA_NAME "form-data; name={key}"
     * @param body  HTTP_FORM_DATA_BODY
     * @return NetworkReply
     */
    QNetworkReply *jsonPostRequest(const QUrl &url, const QJsonDocument &body);

    QNetworkReply *jsonMultiPartRequest(const QUrl &url, const QString &name,
                                        const QJsonDocument &body);

    /**
     * @brief Standard GET Anfrage
     * @note Wenn es eine https Verbindung ist und der
     *    Host mit QUrl:setHost() in der URL gesetzt ist.
     *    Wird SSL Verschlüsselung verwendet (sonst nicht)!
     * @param url
     * @return
     */
    QNetworkReply *jsonGetRequest(const QUrl &url);

    /**
     * @brief Aufräumen
     */
    virtual ~Networker();
  };
};     // namespace Antiqua
#endif // NETWORKER_NETWORKING_H
