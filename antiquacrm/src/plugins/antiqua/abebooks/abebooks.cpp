// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooks.h"
#include "abebooksconfig.h"
#include "abebooksifacewidget.h"
#include "abebooksproviderwidget.h"
#include "abebookspurchaser.h"
#include "abebooksrequester.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QPushButton>
#include <QStyle>
#include <QTabWidget>
#include <QTableWidget>
#include <QTime>
#include <QVBoxLayout>

void AbeBooks::prepareJsonListResponse(const QJsonDocument &doc) {}

void AbeBooks::responseAnswerCheck(const QJsonDocument &doc) {}

bool AbeBooks::createInterface(QObject *parent) {
  if (parent) {
    m_handle = new QObject(parent);
    m_handle->setObjectName("abebooks_plugin");
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

void AbeBooks::queryMenueEntries() { /* TODO */ }

void AbeBooks::updateArticleCount(int articleId, int count) {}
