// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooks.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QPushButton>
#include <QStyle>
#include <QTabWidget>
#include <QTableWidget>
#include <QTime>
#include <QVBoxLayout>

/**
 * @def DATE_FORMAT
 * @ingroup Providers SQL Statements
 * @brief Datumsformat von Buchfreund.de
 * Wird für lesen/schreiben der JSon Datenfelder benötigt.
 */
#ifndef DATE_FORMAT
#define DATE_FORMAT "yyyy-MM-dd hh:mm:ss"
#endif

/** @brief Konfigurationsgruppe */
#ifndef CONFIG_GROUP
#define CONFIG_GROUP "provider/abebooks"
#endif

/** @brief Wird für Menüeintrag und Gruppenzuweisung benötigt! */
#ifndef CONFIG_PROVIDER
#define CONFIG_PROVIDER "AbeBooks"
#endif

#ifndef PLUGIN_ABEBOOKS_DEBUG
#define PLUGIN_ABEBOOKS_DEBUG false
#endif

AbeBooksQuery::AbeBooksQuery(QObject *parent) : QObject{parent} {
  setObjectName("abebooks_json_query");
}

const QUrl AbeBooksQuery::apiQuery(const QString &operation) {
  QRegExp pattern("([\\/]{2,})");
  ApplSettings cfg;

  QUrl url;
  cfg.beginGroup(CONFIG_GROUP);
  url.setScheme(cfg.value("api_scheme").toString());
  url.setHost(cfg.value("api_host").toString());
  QString path(cfg.value("api_basepath").toString());
  path.append("/");
  path.append(cfg.value("api_key").toString());
  path.append("/" + operation);
  cfg.endGroup();
  url.setPath(path.replace(pattern, "/"));
  return url;
}

void AbeBooksQuery::queryList() {}

void AbeBooksQuery::queryOrder(const QString &bfId) {}

void AbeBooksQuery::customQuery(const QString &operation,
                                const QJsonDocument &doc) {}

AbeBooksPurchaser::AbeBooksPurchaser(QWidget *parent)
    : Antiqua::PurchaserWidget{parent} {
  QStyle *wStyle = style();
  QVBoxLayout *customerLayout = new QVBoxLayout(this);
  setLayout(customerLayout);
}

void AbeBooksPurchaser::setCustomerId(int customerId) {}

void AbeBooksPurchaser::setValue(const QString &objName,
                                 const QVariant &value) {}

const QVariant AbeBooksPurchaser::getValue(const QString &objName) {
  return QVariant();
}

AbeBooksProviderWidget::AbeBooksProviderWidget(const QString &widgetId,
                                               QWidget *parent)
    : Antiqua::ProviderWidget{widgetId, parent} {
  setObjectName("buchfreund_main");
  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->addStretch(1);
  setLayout(layout);
}

void AbeBooksProviderWidget::jsonQuery(const QString &operation,
                                       const QJsonDocument &doc) {}

void AbeBooksProviderWidget::updateArticleCount() {}

void AbeBooksProviderWidget::queryResponse(const QJsonDocument &doc) {}

AbeBooksIfaceWidget::AbeBooksIfaceWidget(const QString &widgetId,
                                         QWidget *parent)
    : Antiqua::InterfaceWidget{widgetId, parent} {
  setWidgetResizable(true);
  setObjectName(widgetId);
  setWindowTitle(widgetId);

  QWidget *mainWidget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(mainWidget);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addStretch(1);
  mainWidget->setLayout(layout);
  setWidget(mainWidget);
}

const QVariant AbeBooksIfaceWidget::tableData(int row, int column) {
  return QVariant();
}

void AbeBooksIfaceWidget::createCustomerDocument() {}

const QJsonDocument
AbeBooksIfaceWidget::customerRequest(const QJsonObject &object) {
  return QJsonDocument();
}

void AbeBooksIfaceWidget::parseAddressBody(const QString &section,
                                           const QJsonObject &object) {}

void AbeBooksIfaceWidget::readCurrentArticleIds() {}

void AbeBooksIfaceWidget::setContent(const QJsonDocument &doc) {}

void AbeBooksIfaceWidget::createOrderRequest(const QString &bfId) {}

void AbeBooksIfaceWidget::setCustomerId(int customerId) {}

const QMap<QString, QString> AbeBooksIfaceWidget::fieldTranslate() const {
  QMap<QString, QString> m;
  return m;
}

const ProviderOrder AbeBooksIfaceWidget::getProviderOrder() {
  ProviderOrder o;
  return o;
}

void AbeBooks::prepareJsonListResponse(const QJsonDocument &doc) {}

void AbeBooks::responseAnswerCheck(const QJsonDocument &doc) {}

bool AbeBooks::createInterface(QObject *parent) {
  if (parent) {
    m_abeBooks = new QObject(parent);
    m_abeBooks->setObjectName("abebooks_plugin");
    return true;
  }
  return false;
}

Antiqua::ProviderWidget *AbeBooks::providerWidget(const QString &widgetId,
                                                  QWidget *parent) {
  m_abeBooksProviderWidget = new AbeBooksProviderWidget(widgetId, parent);
  m_abeBooksProviderWidget->setObjectName(widgetId);
  return m_abeBooksProviderWidget;
}

Antiqua::InterfaceWidget *AbeBooks::addWidget(const QString &widgetId,
                                              QWidget *parent) {
  m_abeBooksIfaceWidget = new AbeBooksIfaceWidget(widgetId, parent);
  m_abeBooksIfaceWidget->setObjectName(widgetId);
  return m_abeBooksIfaceWidget;
}

const QString AbeBooks::provider() const { return QString(CONFIG_PROVIDER); }

const QString AbeBooks::configGroup() const { return QString(CONFIG_GROUP); }

void AbeBooks::queryMenueEntries() {
  /* TODO */
}

void AbeBooks::updateArticleCount(int articleId, int count) {}
