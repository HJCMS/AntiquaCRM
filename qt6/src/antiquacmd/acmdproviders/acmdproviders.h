// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACMD_PROVIDERS_H
#define ANTIQUACMD_PROVIDERS_H

#include "acmdconfig.h"
#include <AntiquaCRM>
#include <QDomDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QMetaObject>
#include <QNetworkCookie>
#include <QObject>
#include <QSqlRecord>
#include <QUrl>

#ifndef ANTIQUACMD_INTERFACE
#  define ANTIQUACMD_INTERFACE "de.hjcms.antiquacmd"
#endif

class ACMDPLUGIN_LIBRARY ACmdProviders : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Interface", ANTIQUACMD_INTERFACE)
  Q_CLASSINFO("Description", "AntiquaCMD Providers Interface")
  Q_CLASSINFO("Version", ANTIQUACRM_VERSION)
  Q_CLASSINFO("Url", ANTIQUACRM_HOMEPAGE)

private Q_SLOTS:
  void getNetworkResponse(const QJsonDocument&);
  void getNetworkResponse(const QDomDocument&);

protected:
  /**
   * @brief Basic class members, always initialed in constructor.
   * @{
   */
  AntiquaCRM::ASqlCore* pgsql;
  AntiquaCRM::ASettings* cfg;
  AntiquaCRM::ANetworker* netw;
  /**
   * @}
   */

  /**
   * @brief Predefined - Cookies for temporary caching
   */
  QNetworkCookie actionsCookie;
  QNetworkCookie authenticCookie = QNetworkCookie();

  /**
   * @brief How far in the past can the requests lie?
   *
   * This parameter can be read or set in the configuration with @i
   * General/history_query. Because the server polls every few minutes depending
   * on the cron configuration. If server failures last a maximum of 1-2 days, a
   * value of @i -3 is completely sufficient!
   *
   * @note Is intended for all service providers, the minimum value is "-14"!
   * Values below this are not supported by any service provider!
   *
   * @warning For values below @b -8, Booklooker returns incorrect headers!
   * @default The default value is -3 (One weekend)
   */
  qint8 history_query = -3;

  /**
   * @brief Standard query URL to the service provider
   * @list When creating the URL, please note the following:
   *  @li Scheme and host must be set!
   *  @li The service provider certificate must be included in the CA package!
   *
   * AntiquaCRM does not establish insecure connections and all cookies must
   * also be set to secure!
   *
   * @code
   *  QUrl url;
   *  url.setSheme("https");
   *  url.setHost("hjcms.de");
   * @endcode
   */
  QUrl baseUrl;

  /**
   * @brief Optional - username
   */
  QString apiUser;

  /**
   * @brief An API key is mandatory!
   * @see Settings "provider/{NAME}/api_key"
   */
  QString apiKey;

  /**
   * @brief Optional - URL path
   */
  QString apiPath;

  /**
   * @brief Optional - Service provider query port
   *
   * There is no need to specify 443 for “https”!
   * AntiquaCRM uses an SSL connection by default!
   */
  qint64 apiPort;

  /**
   * @brief max allowed data Length for columns!
   *
   * Contains the maximum character length for columns containing strings.
   */
  QMap<QString, int> initDataInformation();

  /**
   * @brief Service provider name title in upper and lower case!
   *
   * Is used for querying the database and displaying it in the menu tree!
   */
  virtual const QString provider() const = 0;

  /**
   * @brief Method for reading the configuration
   * @note Should be included in @ref init()!
   */
  virtual void initConfiguration() = 0;

  /**
   * @brief Method to create query url
   */
  virtual const QUrl apiQuery(const QString& action) = 0;

  /**
   * @brief The JSon document for the AntiquaCRM import is created here!
   * @sa AntiquaCRM::JSON::Schema
   */
  virtual void prepareContent(const QJsonDocument& document) = 0;

  /**
   * @brief Helper - Correcting incorrect name information.
   *
   * This function prepare some replaces before calling AntiquaCRM::AUtil::ucFirst
   */
  const QString ucFirst(const QString& str);

  /**
   * @brief gender detection
   *
   * The try, to get service provider information about customer gender.
   */
  int convertGender(const QString& gender) const;

  /**
   * @brief IETF BCP 47 language attributes
   */
  const QString findBCP47(const QString& country) const;

  /**
   * @brief Search country with IETF BCP 47 language attribute
   */
  const QString getCountry(const QString& bcp47) const;

  /**
   * @brief Creates QDateTime from QDate and QTime
   */
  const QDateTime getDateTime(const QString& dateString, const QString& timeString,
                              Qt::TimeSpec spec = Qt::LocalTime) const;

  /**
   * @brief Converts QDateTime to AntiquaCRM::ISODate Time
   */
  const QDateTime getDateTime(const QString& dateTimeString,
                              Qt::TimeSpec spec = Qt::LocalTime) const;

  /**
   * @brief Time zones are required for the “cookies”.
   */
  const QDateTime timeSpecDate(const QDateTime& dateTime,
                               Qt::TimeSpec fromSpec = Qt::LocalTime) const;

  /**
   * @brief Helper for the types converting the service provider tasks!
   * @param field - SQL Fieldname
   * @param value - Value to convert
   * @note The method should always be used when importing!
   *
   * For more information see Qt documentation QJsonValue::Type
   */
  const QJsonValue convert(const QString& field, const QJsonValue& value) const;

  /**
   * @brief List of service provider order numbers
   * @param provider - Provider keyname in @b CaseSensitive
   *
   * Returns a list of already imported service provider order numbers. This
   * method @b MUST always be called before @ref createOrders! Otherwise you
   * will get a SQL INSERT error!
   */
  const QStringList currProviderIds(const QString& provider);

  /**
   * @brief Create SQL inserts with AntiquaCRM::JSON::Schema
   * @param orders - Article orders
   * @note Only SQL::INSERT's are executed!
   */
  bool createOrders(const QList<QJsonObject>& orders);

  /**
   * @brief Customer search
   * @param json - AntiquaCRM::JSON::Schema "JsonObject::customer"
   * @note To avoid double spending, "c_provider_import" must always be set -
   * see documentation AntiquaCRM::JSON::Schema!
   * @return QPair<CustomerID, "Customer First- lastname">
   */
  QPair<qint64, QString> findInsertCustomer(const QJsonObject& json);

  /**
   * @brief Query Article type with Article Number
   * @note Providers didn't set article mediaTypes
   * @code
   *  psql -c "SELECT func_get_article_type(articleid);"
   * @endcode
   * @default AntiquaCRM::ArticleType::Book
   */
  AntiquaCRM::ArticleType findArticlType(const QString& aid);

protected Q_SLOTS:
  /**
   * @brief Process network response!
   * @param bread - bytes read
   */
  virtual void responsed(const QByteArray& bread) = 0;

Q_SIGNALS:
  /**
   * @brief Query was unsuccessful or canceled! :-(
   */
  void sendDisjointed();

  /**
   * @brief Query was successful :-)
   */
  void sendFinished();

public Q_SLOTS:
  /**
   * @brief Start service provider process
   * @note Must be called after @ref createInterface()!
   */
  virtual void start() = 0;

public:
  explicit ACmdProviders(AntiquaCRM::NetworkQueryType type, QObject* parent = nullptr);
  ~ACmdProviders();

  /**
   * @brief Load configuration and check parameters!
   * Executed before @ref start() to avoid errors!
   */
  virtual bool init() = 0;
};

Q_DECLARE_INTERFACE(ACmdProviders, ANTIQUACMD_INTERFACE);

#endif // ANTIQUACMD_PROVIDERS_H
