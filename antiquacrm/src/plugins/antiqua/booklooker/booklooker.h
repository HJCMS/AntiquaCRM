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

class BooklookerProviderWidget;
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

  /**
   * @brief BooklookerProviderWidget
   * Antiqua::ProviderWidget
   */
  BooklookerProviderWidget *m_blProviderWidget;

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
  const QString configGroup() const;

  /**
   * @brief Menü Einträge suchen
   */
  void queryMenueEntries();

  /**
   * @brief Artikelbestand ändern!
   * @param articleId
   * @param count
   */
  void updateArticleCount(int articleId, int count);
};

#endif // BOOKLOOKER_PLUGIN_H
