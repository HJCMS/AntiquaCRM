// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PROVIDER_NETWORKING_H
#define PROVIDER_NETWORKING_H

#include <QJsonDocument>
#include <QMetaType>
#include <QMutex>
#include <QObject>
#include <QRunnable>
#include <QTextCodec>
#include <QThread>
#include <QUrl>

#include <AntiquaCRM>

#ifndef CURLINC_CURL_H
#ifndef CURL_NO_OLDIES
#define CURL_NO_OLDIES 1
#endif
#include <curl/curl.h>
#endif

namespace Antiqua {
  /**
   * @brief Thread Prozess für cUrl anfragen!
   * @class ProviderRequest
   * @ingroup Networking
   * @warning Nur mit QThread aufrufen!
   * @see Provider
   * <a href="https://everything.curl.dev/">cUrl</a>
   */
  class ANTIQUACORE_EXPORT ProviderRequest : public QRunnable {

  private:
    /**
     * @brief Variable für cUrl Trace
     */
    bool verbose = false;

    /**
     * @brief cUrl handle
     */
    CURL *m_curl;

    /**
     * @brief System Kodierung einlesen
     */
    QTextCodec *m_txtCodec;
    QString p_charset = QString("utf8");

    /**
     * @brief HTTP Accept-Language (RFC2616)
     * @link https://datatracker.ietf.org/doc/rfc2616/
     */
    const char *acceptLanguage();

    /**
     * @brief HTTP Accept (RFC2616) Json
     * @link https://datatracker.ietf.org/doc/rfc2616/
     */
    const char *acceptJsonLanguage();

    /**
     * @brief HTTP Content-Type Header Json
     */
    const char *contentTypeJson();

    /**
     * @brief HTTP Headers
     */
    curl_slist *setHeader();

    /**
     * @brief Rückgabe in ... Schreiben
     */
    std::string errorBuffer;
    std::string dataBuffer;
    static int writeData(char *data, size_t size, size_t nitems,
                         std::string *buffer);

    /**
     * @brief TODO
     */
    static size_t readFile(char *data, size_t size, size_t nitems,
                           std::string *buffer);

    /**
     * @brief Lese den Datenpuffer in QString
     */
    const QString getBuffer();

    QJsonDocument p_json;
    bool createDocument();

    /**
     * @brief Anfrage Adresse setzen
     */
    void setRemoteUrl(const QUrl &);

    /**
     * @brief Standard cUrl Optionen setzen
     */
    bool initDefaultOptions();

  public:
    explicit ProviderRequest(bool verbose = false);

    /**
     * @brief cUrl Fehlermeldung ausgeben.
     */
    const QString getResponseError();

    /**
     * @brief Json Daten lesen
     */
    const QJsonDocument getResponseData();

    /**
     * @brief Einfache GET Anfrage senden
     */
    bool get(const QUrl &url);

    /**
     * @brief Ein application/json POST Senden
     * @param url  Server Adresse
     * @param body Json Document
     */
    bool post(const QUrl &url, const QByteArray &body);

    /**
     * @brief Multipart Form Data für Bild- /Zipdateien
     * @param url    Server Adresse
     * @param param  form Tag
     * @param body   ByteArray
     */
    bool postForm(const QUrl &url, const QString &param, const QByteArray &body);

    void run() override;

    virtual ~ProviderRequest();
  };

  /**
   * @class Provider
   * @ingroup Networking
   * @brief Alternative Klasse für Json anfragen
   * Ich verwende deshalb an dieser Stelle cUrl weil der QNetworkAccessManager
   * Probleme mit HTTP/2 Redirected hat. Mit cURL habe ich hier bessere
   * Erfahrungen gemacht und setze es deshalb ein.
   */
  class Provider : public QThread {
    Q_OBJECT
    Q_CLASSINFO("Author", "Jürgen Heinemann")
    Q_CLASSINFO("URL", "https://www.hjcms.de")

  private:
    bool verbose = false;
    qint64 p_type = 0x000000;
    QMutex p_mutex;
    QUrl p_url;
    QByteArray p_body;
    QString p_formdata;
    QJsonDocument p_json;

    void run() override;

  Q_SIGNALS:
    /**
     * @brief Einfache Fehlermeldung ausgeben
     */
    void errors(const QString &);

    /**
     * @brief Optional zu Signal requestFinished
     *  Noch die Daten auf einen Slot geben.
     */
    void responsed(const QJsonDocument &);

    /**
     * @brief Wenn Daten empfangen wurden.
     */
    void requestFinished(bool);

  public:
    enum Type { GET = 0x000001, POST = 0x000010, FORM = 0x000100 };
    Q_ENUM(Type)

    explicit Provider(QObject *parent = nullptr, bool debug = false);

    /**
     * @brief Einfache GET Anfrage senden
     */
    void sendGet(const QUrl &url);

    /**
     * @brief Ein application/json POST Senden
     * @param url  Server Adresse
     * @param body Json Document
     */
    void sendPost(const QUrl &url, const QByteArray &body);

    void sendPostForm(const QUrl &url, const QString &param,
                      const QByteArray &body);

    const QJsonDocument getDocument();
  };
};     // namespace Antiqua
#endif // PROVIDER_NETWORKING_H
