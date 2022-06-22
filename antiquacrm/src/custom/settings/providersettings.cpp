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
  m_whsoft->setCheckable(true);
  m_whsoft->setChecked(false);
  m_whsoft->setObjectName("whsoft");
  m_whsoft->setTitle("W+H Software (buchfreund.de)");
  QVBoxLayout *whSoftLayout = new QVBoxLayout(m_whsoft);
  whSoftLayout->setObjectName("provider_whsoft_layout");
  m_whsoft_scheme = new LineEdit(m_whsoft);
  m_whsoft_scheme->setObjectName("api_scheme");
  m_whsoft_scheme->setInfo(tr("Protocoll"));
  m_whsoft_scheme->setValue("https");
  whSoftLayout->addWidget(m_whsoft_scheme);
  m_whsoft_api_host = new LineEdit(m_whsoft);
  m_whsoft_api_host->setObjectName("api_host");
  m_whsoft_api_host->setInfo(tr("Domain"));
  m_whsoft_api_host->setValue("www.buchfreund.de");
  whSoftLayout->addWidget(m_whsoft_api_host);
  m_whsoft_api_key = new LineEdit(m_whsoft);
  m_whsoft_api_key->setObjectName("api_key");
  m_whsoft_api_key->setInfo(tr("API Key"));
  whSoftLayout->addWidget(m_whsoft_api_key);
  m_whsoft_api_basepath = new LineEdit(m_whsoft);
  m_whsoft_api_basepath->setObjectName("api_basepath");
  m_whsoft_api_basepath->setInfo(tr("API Path"));
  m_whsoft_api_basepath->setValue("/verkaeufer/api/");
  whSoftLayout->addWidget(m_whsoft_api_basepath);
  whSoftLayout->addStretch(1);
  m_whsoft->setLayout(whSoftLayout);
  layout->addWidget(m_whsoft);
  // END

  // BEGIN AbeBooks
  m_abebooks = new QGroupBox(this);
  m_abebooks->setCheckable(true);
  m_abebooks->setChecked(false);
  m_abebooks->setObjectName("whsoft");
  m_abebooks->setTitle("AbeBooks & ZVAB (abebooks.de)");
  QVBoxLayout *abeBooksLayout = new QVBoxLayout(m_abebooks);
  abeBooksLayout->setObjectName("provider_abebooks_layout");
  m_abebooks_user = new LineEdit(m_abebooks);
  m_abebooks_user->setObjectName("api_user");
  m_abebooks_user->setInfo(tr("Loginname"));
  abeBooksLayout->addWidget(m_abebooks_user);
  m_abebooks_password = new LineEdit(m_abebooks);
  m_abebooks_password->setObjectName("api_password");
  m_abebooks_password->setInfo(tr("Password"));
  m_abebooks_password->setPasswordInput(true);
  abeBooksLayout->addWidget(m_abebooks_password);
  m_abebooks_scheme = new LineEdit(m_abebooks);
  m_abebooks_scheme->setObjectName("api_scheme");
  m_abebooks_scheme->setInfo(tr("Protocoll"));
  m_abebooks_scheme->setValue("https");
  abeBooksLayout->addWidget(m_abebooks_scheme);
  m_abebooks_api_host = new LineEdit(m_abebooks);
  m_abebooks_api_host->setObjectName("api_host");
  m_abebooks_api_host->setInfo(tr("Domain"));
  m_abebooks_api_host->setValue("orderupdate.abebooks.com");
  abeBooksLayout->addWidget(m_abebooks_api_host);
  m_abebooks_api_port = new LineEdit(m_abebooks);
  m_abebooks_api_port->setObjectName("api_port");
  m_abebooks_api_port->setInfo(tr("API Port"));
  m_abebooks_api_port->setValue(10003);
  abeBooksLayout->addWidget(m_abebooks_api_port);
  m_abebooks_api_key = new LineEdit(m_abebooks);
  m_abebooks_api_key->setObjectName("api_key");
  m_abebooks_api_key->setInfo(tr("API Key"));
  abeBooksLayout->addWidget(m_abebooks_api_key);
  abeBooksLayout->addStretch(1);
  m_abebooks->setLayout(abeBooksLayout);
  layout->addWidget(m_abebooks);
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

void ProviderSettings::setPassword(LineEdit *e, const QByteArray &data) {
  QByteArray pw = QByteArray::fromBase64(data, QByteArray::Base64UrlEncoding);
  e->setValue(QString(pw));
}

const QByteArray ProviderSettings::passwordToBase64(LineEdit *e) {
  return e->value().toByteArray().toBase64(QByteArray::Base64Encoding);
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
  // W+H Soft
  config->beginGroup("provider/whsoft");
  QStringList whSoftKeys = config->allKeys();
  m_whsoft->setChecked((whSoftKeys.count() == 4));
  foreach (QString s, whSoftKeys) {
    LineEdit *e =
        m_whsoft->findChild<LineEdit *>(s, Qt::FindDirectChildrenOnly);
    if (e != nullptr) {
      e->setValue(config->value(s));
    }
  }
  config->endGroup();
  // AbeBooks
  config->beginGroup("provider/abebooks");
  QStringList abeBookKeys = config->allKeys();
  m_abebooks->setChecked((abeBookKeys.count() >= 4));
  foreach (QString s, abeBookKeys) {
    LineEdit *e =
        m_abebooks->findChild<LineEdit *>(s, Qt::FindDirectChildrenOnly);
    if (e != nullptr) {
      if (s.contains("api_pass"))
        setPassword(e, config->value(s).toByteArray());
      else
        e->setValue(config->value(s));
    }
  }
  config->endGroup();

  initSignalChanged();
}

void ProviderSettings::saveSectionConfig() {
  if (!m_whsoft->isChecked()) {
    config->remove("provider/whsoft");
  } else {
    config->beginGroup("provider/whsoft");
    QList<LineEdit *> l = m_whsoft->findChildren<LineEdit *>(
        QString(), Qt::FindDirectChildrenOnly);
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
  if (!m_abebooks->isChecked()) {
    config->remove("provider/abebooks");
  } else {
    config->beginGroup("provider/abebooks");
    QList<LineEdit *> l = m_abebooks->findChildren<LineEdit *>(
        QString(), Qt::FindDirectChildrenOnly);
    if (l.count() > 0) {
      for (int i = 0; i < l.count(); i++) {
        LineEdit *e = l.at(i);
        if (e != nullptr) {
          if (e->objectName().contains("api_pass"))
            config->setValue(e->objectName(), passwordToBase64(e));
          else
            config->setValue(e->objectName(), e->value().toString());
        }
      }
    }
    config->endGroup();
  }
}
