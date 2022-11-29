// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PLUGIN_INTERFACE_H
#define ANTIQUACRM_PLUGIN_INTERFACE_H

#include <AntiquaCRM>
#include <QDate>
#include <QDateTime>
#include <QDomDocument>
#include <QFlags>
#include <QJsonDocument>
#include <QJsonValue>
#include <QMetaType>
#include <QNetworkCookie>
#include <QObject>
#include <QString>
#include <QTextCodec>
#include <QUrl>

namespace AntiquaCRM {

/**
 * @brief Primary Plugin Interface class
 * @ingroup OrderSystem
 * @section PluginInterface
 */
class ANTIQUACRM_LIBRARY APluginInterface : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Interface", ANTIQUACRM_INTERFACE)
  Q_CLASSINFO("Description", "AntiquaCRM Plugin Interface")
  Q_CLASSINFO("Version", ANTIQUACRM_VERSION)
  Q_CLASSINFO("Url", ANTIQUACRM_HOMEPAGE)

protected:
  AntiquaCRM::ANetworker *m_network;
  QTextCodec *m_decodeFrom;

  /**
   * @brief API access Remote URL
   */
  QUrl apiUrl;

  /**
   * @brief API access Loginname
   */
  QString apiUser;

  /**
   * @brief API access Key
   */
  QString apiKey;

  /**
   * @brief Actions Session Cookie
   */
  QNetworkCookie actionsCookie;

  /**
   * @brief Authentication Cookie
   */
  QNetworkCookie authenticCookie = QNetworkCookie();

  /**
   * @brief Convert Gender from String to AntiquaCRM::Gender
   */
  AntiquaCRM::Gender convertGender(const QString &from) const;

  /**
   * @brief Search for IETF BCP 47 Language tag with Country name
   * @return "IETF BCP 47 language tag"
   */
  const QString bcp47Country(const QString &country) const;

  /**
   * @brief Search for Country name with IETF BCP 47 language tag
   */
  const QString getCountry(const QString &bcp47) const;

  /**
   * @brief Vendors respond - with different date/time and zone formats.
   */
  const QDateTime getDateTime(const QString &dateString,
                              const QString &timeString,
                              Qt::TimeSpec spec = Qt::LocalTime) const;

  const QDateTime getDateTime(const QString &dateTimeString,
                              Qt::TimeSpec spec = Qt::LocalTime) const;

  /**
   * @brief Convert Datetime to TimeSpec
   */
  const QDateTime timeSpecDate(const QDateTime &dateTime,
                               Qt::TimeSpec fromSpec = Qt::LocalTime) const;

  /**
   * @brief Convert Invalid Price formats to double
   */
  double getPrice(const QJsonValue &value) const;

  /**
   * @brief First letter uppercase
   */
  const QString ucFirst(const QString &name) const;

  /**
   * @brief Vendors using different Date time formats!
   */
  virtual const QString
  dateString(const QDate &date = QDate::currentDate()) const = 0;

  /**
   * @brief load API access configuration
   */
  virtual bool initConfigurations() = 0;

  /**
   * @brief create a custom API access
   */
  virtual const QUrl apiQuery(const QString &section) = 0;

  /**
   * @brief create order item value
   */
  virtual const QVariant createValue(QMetaType::Type id,
                                     const QJsonValue &value) const = 0;

  /**
   * @brief Set AProviderOrder Item
   */
  virtual void setOrderItem(AntiquaCRM::AProviderOrder *order,
                            const QString &key,
                            const QJsonValue &value) const = 0;

  /**
   * @brief Set AProviderOrder Item
   */
  virtual const AntiquaCRM::ArticleOrderItem
  setArticleItem(AntiquaCRM::AProviderOrder *order, const QString &key,
                 const QJsonValue &value) const = 0;

protected Q_SLOTS:
  /**
   * @brief Authentications if needed
   */
  virtual void authenticate() = 0;

  /**
   * @brief Changes the primary Text Codec.
   * @ref m_decodeFrom
   */
  void setContentDecoder(QTextCodec *);

  /**
   * @brief This functions are called to preparing Response Data from given
   * Vendors.
   */
  virtual void prepareResponse(const QJsonDocument &js) = 0;
  virtual void prepareResponse(const QDomDocument &xml) = 0;

  /**
   * @brief This function is called when Network request has been finished.
   */
  virtual void queryFinished(QNetworkReply *reply) = 0;

Q_SIGNALS:
  /**
   * @brief This Signal is reserved to emitted when a Network or Parser error
   * has occurred.
   */
  void sendErrorResponse(AntiquaCRM::Message, const QString &);

  /**
   * @brief This Signal is reserved to emitted when all operations has been
   * finished.
   */
  void sendQueryFinished();

  /**
   * @brief This Signal is reserved to emitted when no data emitted
   */
  void sendQueryAborted();

public Q_SLOTS:
  /**
   * @brief This Method is reserved to Query New Orders.
   */
  virtual void queryNewOrders() = 0;

  /**
   * @brief Authentication is Required?
   * Returns Information about a required Authentication.
   * This is not the general login behavior.
   * It's needed for Session based Connections.
   * @example expired Tokens
   */
  virtual bool authenticationRequired() = 0;

  /**
   * @brief Order single query
   * This Method is reserved to Query with a given Provider Order Id.
   */
  virtual void queryOrder(const QString &orderId) = 0;

  /**
   * @brief Send Operation to Provider Interface
   * @param section  - Operation category
   * @param operation - JsonObject
   */
  virtual void orderUpdateAction(const QJsonObject &options) = 0;

public:
  explicit APluginInterface(QObject *parent = nullptr);

  /**
   * @brief This function read the Remote access configuration from the
   * Application. The Description can be found in APluginConfig Class.
   * @ref AntiquaCRM::APluginConfig
   */
  const AntiquaCRM::APluginConfig getConfig(const QString &providerName);

  /**
   * @brief Returning the Provider Configuration section.
   * @note It is important, that the Group Key 'provider' is followed by a
   * Slash, before add the Vendor name.
   * @example "provider/my_vendor_name"
   */
  virtual const QString configProvider() const = 0;

  /**
   * @brief Returning the Provider Registry and Displayname.
   */
  virtual const QString displayName() const = 0;

  /**
   * @brief Returns all new Orders from finished network query.
   * @note Read the restriction in the AntiquaCRM::AProviderOrders Class!
   * @ref AntiquaCRM::AProviderOrders
   */
  virtual const AntiquaCRM::AProviderOrders getOrders() const = 0;

  /**
   * @brief Initial this Interface
   */
  virtual bool createInterface(QObject *parent) = 0;
};

}; // namespace AntiquaCRM

Q_DECLARE_INTERFACE(AntiquaCRM::APluginInterface, ANTIQUACRM_INTERFACE)

#endif // ANTIQUACRM_PLUGIN_INTERFACE_H
