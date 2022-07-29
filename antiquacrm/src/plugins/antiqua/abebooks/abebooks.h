// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKS_PLUGIN_H
#define ABEBOOKS_PLUGIN_H

#include <QDomDocument>
#include <QObject>
#include <QVariant>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

class AbeBooksIfaceWidget;

/**
 * @class AbeBooks
 * @group AbeBooks Plugin
 * https://help.abebooks.com/abebooks-apis/
 */
class ANTIQUACORE_EXPORT AbeBooks : public Antiqua::Interface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "de.hjcms.antiquacrm.AntiquaFactoryInterface" FILE
                        "abebooks.json")
  Q_INTERFACES(Antiqua::Interface)

private:
  QObject *m_handle;

  /**
   * @brief AbeBooksIfaceWidget
   * Antiqua::InterfaceWidget
   */
  AbeBooksIfaceWidget *m_abeBooksIfaceWidget;

private Q_SLOTS:
  void prepareJsonListResponse(const QJsonDocument &) { /* unused */
  }
  void responseImport(const QDomDocument &);

public:
  bool createInterface(QObject *parent);

  /**
   * @brief Bestellungen
   */
  Antiqua::InterfaceWidget *addWidget(const QString &orderId, QWidget *parent);

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

#endif // ABEBOOKS_PLUGIN_H
