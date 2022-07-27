// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKER_PLUGIN_H
#define BOOKLOOKER_PLUGIN_H

#include <QDomDocument>
#include <QObject>
#include <QVariant>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

class BooklookerIfaceWidget;

/**
 * @class Booklooker
 * @group Booklooker Plugin
 * REST API v2.0
 * https://www.booklooker.de/pages/rest_api.php
 */
class ANTIQUACORE_EXPORT Booklooker : public Antiqua::Interface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "de.hjcms.antiquacrm.AntiquaFactoryInterface" FILE
                        "booklooker.json")
  Q_INTERFACES(Antiqua::Interface)

private:
  QObject *m_handle;

  /**
   * @brief BooklookerIfaceWidget
   * Antiqua::InterfaceWidget
   */
  BooklookerIfaceWidget *m_blIfaceWidget;

private Q_SLOTS:
  void prepareJsonListResponse(const QJsonDocument &);

public:
  bool createInterface(QObject *parent);

  /**
   * @brief Bestellungen
   */
  Antiqua::InterfaceWidget *addWidget(const QString &widgetId, QWidget *parent);

  const QString provider() const;

  /**
   * @brief Wird für Anwender Konfiguration benötigt
   */
  const QString configGroup() const;

  /**
   * @brief Menü Einträge suchen
   */
  void queryMenueEntries();

  /**
   * @brief Versandbestätigung
   */
  void updateOrderDelivery(const QJsonObject &jso);

  /**
   * @brief Artikelbestand ändern!
   * @param articleId
   * @param count
   */
  void updateArticleCount(int articleId, int count);

  /**
   * @brief Bild Hochladen
   */
  void uploadArticleImage(int articleId, const QString &base64);
};

#endif // BOOKLOOKER_PLUGIN_H
