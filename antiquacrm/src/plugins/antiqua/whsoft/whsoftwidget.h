// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef WHSOFTWIDGET_PLUGIN_H
#define WHSOFTWIDGET_PLUGIN_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QVariant>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

/**
 * @brief In dieser Klasse werden die Bestellungen verarbeitet.
 * @ingroup Whsoft Plugin
 */
class ANTIQUACORE_EXPORT WHSoftWidget final : public Antiqua::InterfaceWidget {
  Q_OBJECT

private:
  QHash<QString, QString> p_customer;
  QJsonDocument p_currentDocument;

  void setOrderUpdateTypes();

  /**
   * @brief Erstellt abfrage Datensatz für Kundenabfrage
   */
  const QJsonDocument customerRequest(const QJsonObject &object);

  /**
   * @brief Rechnungs und Lieferadressen einlesen
   */
  void parseAddressBody(const QString &section, const QJsonObject &object);

private Q_SLOTS:
  void checkCustomerExists();
  void readCurrentArticleIds();

public Q_SLOTS:
  void createCustomerDocument();
  void setContent(const QJsonDocument &);
  void createOrderRequest();
  void createProviderOrderUpdate();

public:
  WHSoftWidget(const QString &orderId, QWidget *parent = nullptr);

  /**
   * @brief Übersetzt die Buchfreund.de Json Datenfelder zu SQL Spaltenname.
   * Wir verwenden andere Datenfeld Bezeichnungen als der Dienstanbieter.
   * Deshalb müssen die Parameter vor der Weiterleitung an dieser Stelle
   * übersetzt werden.
   */
  const QMap<QString, QString> fieldTranslate() const;
};

#endif // WHSOFTWIDGET_PLUGIN_H
