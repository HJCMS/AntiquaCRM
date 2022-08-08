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
  QVBoxLayout *m_whsoft_layout = new QVBoxLayout(m_whsoft);
  m_whsoft_layout->setObjectName("provider_whsoft_layout");
  m_whsoft_scheme = new LineEdit(m_whsoft);
  m_whsoft_scheme->setObjectName("api_scheme");
  m_whsoft_scheme->setInfo(tr("Protocoll"));
  m_whsoft_scheme->setValue("https");
  m_whsoft_layout->addWidget(m_whsoft_scheme);
  m_whsoft_api_host = new LineEdit(m_whsoft);
  m_whsoft_api_host->setObjectName("api_host");
  m_whsoft_api_host->setInfo(tr("Domain"));
  m_whsoft_api_host->setValue("www.buchfreund.de");
  m_whsoft_layout->addWidget(m_whsoft_api_host);
  m_whsoft_api_key = new LineEdit(m_whsoft);
  m_whsoft_api_key->setObjectName("api_key");
  m_whsoft_api_key->setInfo(tr("API Key"));
  m_whsoft_layout->addWidget(m_whsoft_api_key);
  m_whsoft_api_basepath = new LineEdit(m_whsoft);
  m_whsoft_api_basepath->setObjectName("api_basepath");
  m_whsoft_api_basepath->setInfo(tr("API Path"));
  m_whsoft_api_basepath->setValue("/verkaeufer/api/");
  m_whsoft_layout->addWidget(m_whsoft_api_basepath);
  m_whsoft_layout->addStretch(1);
  m_whsoft->setLayout(m_whsoft_layout);
  layout->addWidget(m_whsoft);
  // END

  // BEGIN AbeBooks
  m_abebooks = new QGroupBox(this);
  m_abebooks->setCheckable(true);
  m_abebooks->setChecked(false);
  m_abebooks->setObjectName("abebooks");
  m_abebooks->setTitle("AbeBooks && ZVAB (abebooks.de)");
  QVBoxLayout *m_abe_layout = new QVBoxLayout(m_abebooks);
  m_abe_layout->setObjectName("provider_abebooks_layout");
  m_abebooks_user = new LineEdit(m_abebooks);
  m_abebooks_user->setObjectName("api_user");
  m_abebooks_user->setInfo(tr("Loginname"));
  m_abe_layout->addWidget(m_abebooks_user);
  m_abebooks_password = new LineEdit(m_abebooks);
  m_abebooks_password->setObjectName("api_password");
  m_abebooks_password->setInfo(tr("Password"));
  m_abebooks_password->setPasswordInput(true);
  m_abe_layout->addWidget(m_abebooks_password);
  m_abebooks_scheme = new LineEdit(m_abebooks);
  m_abebooks_scheme->setObjectName("api_scheme");
  m_abebooks_scheme->setInfo(tr("Protocoll"));
  m_abebooks_scheme->setValue("https");
  m_abe_layout->addWidget(m_abebooks_scheme);
  m_abebooks_api_host = new LineEdit(m_abebooks);
  m_abebooks_api_host->setObjectName("api_host");
  m_abebooks_api_host->setInfo(tr("Domain"));
  m_abebooks_api_host->setValue("orderupdate.abebooks.com");
  m_abe_layout->addWidget(m_abebooks_api_host);
  m_abebooks_api_port = new LineEdit(m_abebooks);
  m_abebooks_api_port->setObjectName("api_port");
  m_abebooks_api_port->setInfo(tr("API Port"));
  m_abebooks_api_port->setValue(10003);
  m_abe_layout->addWidget(m_abebooks_api_port);
  m_abebooks_api_key = new LineEdit(m_abebooks);
  m_abebooks_api_key->setObjectName("api_key");
  m_abebooks_api_key->setInfo(tr("API Key"));
  m_abe_layout->addWidget(m_abebooks_api_key);
  m_abe_layout->addStretch(1);
  m_abebooks->setLayout(m_abe_layout);
  layout->addWidget(m_abebooks);
  // END

  // BEGIN Booklooker
  m_booklooker = new QGroupBox(this);
  m_booklooker->setCheckable(true);
  m_booklooker->setChecked(false);
  m_booklooker->setObjectName("booklooker");
  m_booklooker->setTitle("Booklooker (booklooker.de)");
  QVBoxLayout *m_bl_layout = new QVBoxLayout(m_booklooker);
  m_booklooker_user = new LineEdit(m_booklooker);
  m_booklooker_user->setObjectName("api_user");
  m_booklooker_user->setInfo(tr("Loginname"));
  m_bl_layout->addWidget(m_booklooker_user);
  m_booklooker_scheme = new LineEdit(m_booklooker);
  m_booklooker_scheme->setObjectName("api_scheme");
  m_booklooker_scheme->setInfo(tr("Protocoll"));
  m_booklooker_scheme->setValue("https");
  m_bl_layout->addWidget(m_booklooker_scheme);
  m_booklooker_api_host = new LineEdit(m_booklooker);
  m_booklooker_api_host->setObjectName("api_host");
  m_booklooker_api_host->setInfo(tr("Domain"));
  m_booklooker_api_host->setValue("api.booklooker.de");
  m_bl_layout->addWidget(m_booklooker_api_host);
  m_booklooker_api_key = new LineEdit(m_booklooker);
  m_booklooker_api_key->setObjectName("api_key");
  m_booklooker_api_key->setInfo(tr("API Key"));
  m_bl_layout->addWidget(m_booklooker_api_key);
  m_booklooker->setLayout(m_bl_layout);
  layout->addWidget(m_booklooker);
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
  QList<QGroupBox *> groups =
      findChildren<QGroupBox *>(QString(), Qt::FindDirectChildrenOnly);
  if (groups.size() > 0) {
    QListIterator<QGroupBox *> it(groups);
    while (it.hasNext()) {
      QGroupBox *box = it.next();
      if (box == nullptr)
        continue;

      QString section(box->objectName());
      if (section.isEmpty())
        continue;

      config->beginGroup("provider/" + section);
      QStringList keys = config->allKeys();
      box->setChecked((keys.count() >= 4));
      foreach (QString s, keys) {
        LineEdit *e = box->findChild<LineEdit *>(s, Qt::FindDirectChildrenOnly);
        if (e != nullptr) {
          if (e->isPasswordInput()) {
            setPassword(e, config->value(s).toByteArray());
          } else {
            e->setValue(config->value(s));
          }
        }
      }
      config->endGroup();
    }
  }

  initSignalChanged();
}

void ProviderSettings::saveSectionConfig() {
  QList<QGroupBox *> groups =
      findChildren<QGroupBox *>(QString(), Qt::FindDirectChildrenOnly);
  if (groups.size() > 0) {
    QListIterator<QGroupBox *> it(groups);
    while (it.hasNext()) {
      QGroupBox *box = it.next();
      if (box == nullptr)
        continue;

      QString section(box->objectName());
      if (section.isEmpty())
        continue;

      if (!box->isChecked()) {
        config->remove("provider/" + section);
        continue;
      }

      config->beginGroup("provider/" + section);
      QList<LineEdit *> l =
          box->findChildren<LineEdit *>(QString(), Qt::FindDirectChildrenOnly);
      if (l.count() > 0) {
        for (int i = 0; i < l.count(); i++) {
          LineEdit *e = l.at(i);
          if (e != nullptr) {
            if (e->isPasswordInput()) {
              config->setValue(e->objectName(), passwordToBase64(e));
            } else {
              config->setValue(e->objectName(), e->value());
            }
          }
        }
      }
      config->endGroup();
    }
  }
}
