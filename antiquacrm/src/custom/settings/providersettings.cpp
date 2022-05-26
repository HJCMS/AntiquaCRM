// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providersettings.h"

#include <QGroupBox>
#include <QSizePolicy>
#include <QVBoxLayout>

ProviderSettings::ProviderSettings(QWidget *parent) : SettingsWidget{parent} {
  setObjectName("provider_settings");
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("provider_config_layout");
  // BEGIN W+H Software
  m_whsoft = new QGroupBox(this);
  m_whsoft->setObjectName("whsoft");
  m_whsoft->setTitle("W+H Software (buchfreund.de)");
  QVBoxLayout *box1Layout = new QVBoxLayout(m_whsoft);
  box1Layout->setObjectName("provider_whsoft_layout");
  m_whsoft_scheme = new LineEdit(m_whsoft);
  m_whsoft_scheme->setObjectName("api_scheme");
  m_whsoft_scheme->setInfo(tr("Protocoll"));
  m_whsoft_scheme->setValue("https");
  box1Layout->addWidget(m_whsoft_scheme);
  m_whsoft_api_host = new LineEdit(m_whsoft);
  m_whsoft_api_host->setObjectName("api_host");
  m_whsoft_api_host->setInfo(tr("Domain"));
  m_whsoft_api_host->setValue("buchfreund.de");
  box1Layout->addWidget(m_whsoft_api_host);
  m_whsoft_api_key = new LineEdit(m_whsoft);
  m_whsoft_api_key->setObjectName("api_key");
  m_whsoft_api_key->setInfo(tr("API Key"));
  box1Layout->addWidget(m_whsoft_api_key);
  m_whsoft_api_basepath = new LineEdit(m_whsoft);
  m_whsoft_api_basepath->setObjectName("api_basepath");
  m_whsoft_api_basepath->setInfo(tr("API Path"));
  m_whsoft_api_basepath->setValue("/verkaeufer/api/");
  box1Layout->addWidget(m_whsoft_api_basepath);
  box1Layout->addStretch(1);
  m_whsoft->setLayout(box1Layout);
  layout->addWidget(m_whsoft);
  // END

  layout->addStretch(1);
  setLayout(layout);
}

void ProviderSettings::initSignalChanged() {
  QList<UtilsMain *> l =
      findChildren<UtilsMain *>(QString(), Qt::FindChildrenRecursively);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      UtilsMain *w = l.at(i);
      if (w != nullptr) {
        connect(w, SIGNAL(hasModified(bool)), this, SLOT(chieldModified(bool)));
      }
    }
  }
}

void ProviderSettings::setPageTitle(const QString &title) {
  pageTitle = title;
  emit pageTitleChanged();
}

const QString ProviderSettings::getPageTitle() { return pageTitle; }

void ProviderSettings::setPageIcon(const QIcon &icon) {
  pageIcon = icon;
  emit pageIconChanged();
}

const QIcon ProviderSettings::getPageIcon() { return pageIcon; }

void ProviderSettings::loadSectionConfig() {
  config->beginGroup("provider/whsoft");
  QStringList keys = config->allKeys();
  foreach (QString s, keys) {
    LineEdit *e =
        m_whsoft->findChild<LineEdit *>(s, Qt::FindDirectChildrenOnly);
    if (e != nullptr) {
      e->setValue(config->value(s));
    }
  }
  config->endGroup();
  initSignalChanged();
}

void ProviderSettings::saveSectionConfig() {
  config->beginGroup("provider/whsoft");
  QList<LineEdit *> l =
      m_whsoft->findChildren<LineEdit *>(QString(), Qt::FindDirectChildrenOnly);
  if (l.count() > 0) {
    for (int i = 0; i < l.count(); i++) {
      LineEdit *e = l.at(i);
      if (e != nullptr) {
        config->setValue(e->objectName(), e->value());
      }
    }
  }
  config->endGroup();
}
