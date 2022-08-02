// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#ifndef ANTIQUA_INTERFACE_H
#define ANTIQUA_INTERFACE_H

#include <QJsonDocument>
#include <QObject>
#include <QString>
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
 * @group Antiqua Plugin Interface
 */
namespace Antiqua {

  class InterfaceWidget;

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
    /**
     * @brief Wenn Bestellabfragen erfolgreich waren...
     * @see prepareJsonListResponse
     */
    void s_listResponse(const QJsonDocument &);
    void s_errorResponse(Antiqua::ErrorStatus, const QString &);
    void s_queryResponse(bool successfull);
    void s_anErrorOccurred();

  public:
    virtual bool createInterface(QObject * parent) = 0;

    /**
     * @brief Tab das eingefügt wird wenn Daten empfangen werden!
     * Die einzelnen Tabs werden über den Objektnamen Identifiziert und sind deshalb Unique.
     * @note widgetId muss Identisch mit "objectName" und "tabTitle" sein!
     */
    virtual Antiqua::InterfaceWidget *addWidget(const QString &orderId, QWidget * parent) = 0;

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
     * @brief Bestellungen beantworten
     */
    virtual void updateOrderDelivery(const QJsonObject &jso) = 0;

    /**
     * @brief Artikelbestands Änderungen senden.
     */
    virtual void updateArticleCount(int articleId, int count) = 0;

    /**
     * @brief Bild für Artikelnummer hochladen.
     * @param articleId - Artikelnummer
     * @param data  - Base64 kodierte Zeichenkette
     */
    virtual void uploadArticleImage(int articleId, const QString &base64) = 0;
  };
};

Q_DECLARE_INTERFACE(Antiqua::Interface, ANTIQUA_INTERFACE)

#endif // ANTIQUA_INTERFACE_H
