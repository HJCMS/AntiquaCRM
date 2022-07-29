// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_WHSOFT_PLUGIN_H
#define ANTIQUA_WHSOFT_PLUGIN_H

#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QVariant>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

class WHSoftJSonQuery;
class WHSoftPurchaser;
class WHSoftWidget;

class ANTIQUACORE_EXPORT WHSoft : public Antiqua::Interface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "de.hjcms.antiquacrm.AntiquaFactoryInterface" FILE
                        "whsoft.json")
  Q_INTERFACES(Antiqua::Interface)

private:
  QObject *m_whsoft;
  WHSoftWidget *m_whsoftWidget;   /**< Antiqua::InterfaceWidget */

private Q_SLOTS:
  void prepareJsonListResponse(const QJsonDocument &);
  void responseAnswerCheck(const QJsonDocument &);

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

#endif // ANTIQUA_WHSOFT_PLUGIN_H
