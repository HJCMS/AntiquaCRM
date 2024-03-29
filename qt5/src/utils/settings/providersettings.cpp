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
  m_buchfreund = new QGroupBox(this);
  m_buchfreund->setCheckable(true);
  m_buchfreund->setChecked(false);
  m_buchfreund->setObjectName("buchfreund");
  m_buchfreund->setTitle("Buchfreund (W+H Software)");
  QVBoxLayout *m_buchfreund_layout = new QVBoxLayout(m_buchfreund);
  m_buchfreund_layout->setObjectName("buchfreund_layout");
  m_buchfreund_api_url = new LineEdit(m_buchfreund);
  m_buchfreund_api_url->setObjectName("api_host");
  m_buchfreund_api_url->setInfo(tr("Domain"));
  m_buchfreund_api_url->setValue("www.buchfreund.de");
  m_buchfreund_layout->addWidget(m_buchfreund_api_url);
  m_buchfreund_api_key = new LineEdit(m_buchfreund);
  m_buchfreund_api_key->setObjectName("api_key");
  m_buchfreund_api_key->setInfo(tr("API Key"));
  m_buchfreund_layout->addWidget(m_buchfreund_api_key);
  m_buchfreund_layout->addStretch(1);
  m_buchfreund->setLayout(m_buchfreund_layout);
  layout->addWidget(m_buchfreund);
  // END

  // BEGIN AbeBooks
  m_abebooks = new QGroupBox(this);
  m_abebooks->setCheckable(true);
  m_abebooks->setChecked(false);
  m_abebooks->setObjectName("abebooks");
  m_abebooks->setTitle("AbeBooks && ZVAB (abebooks.de)");
  QVBoxLayout *m_abe_layout = new QVBoxLayout(m_abebooks);
  m_abe_layout->setObjectName("provider_abebooks_layout");
  m_abebooks_api_user = new LineEdit(m_abebooks);
  m_abebooks_api_user->setObjectName("api_user");
  m_abebooks_api_user->setInfo(tr("Loginname"));
  m_abe_layout->addWidget(m_abebooks_api_user);
  m_abebooks_api_url = new LineEdit(m_abebooks);
  m_abebooks_api_url->setObjectName("api_host");
  m_abebooks_api_url->setInfo(tr("Domain"));
  m_abebooks_api_url->setValue("orderupdate.abebooks.com");
  m_abe_layout->addWidget(m_abebooks_api_url);
  m_abebooks_api_port = new LineEdit(m_abebooks);
  m_abebooks_api_port->setObjectName("api_port");
  m_abebooks_api_port->setInfo(tr("API Port"));
  m_abebooks_api_port->setValue(10003);
  m_abe_layout->addWidget(m_abebooks_api_port);
  m_abebooks_api_key = new LineEdit(m_abebooks);
  m_abebooks_api_key->setObjectName("api_key");
  m_abebooks_api_key->setInfo(tr("API Key"));
  m_abe_layout->addWidget(m_abebooks_api_key);
  m_abebooks_seller_id = new LineEdit(m_abebooks);
  m_abebooks_seller_id->setObjectName("seller_id");
  m_abebooks_seller_id->setInfo(tr("Seller Id"));
  m_abe_layout->addWidget(m_abebooks_seller_id);
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
  m_booklooker_api_url = new LineEdit(m_booklooker);
  m_booklooker_api_url->setObjectName("api_host");
  m_booklooker_api_url->setInfo(tr("Domain"));
  m_booklooker_api_url->setValue("api.booklooker.de");
  m_bl_layout->addWidget(m_booklooker_api_url);
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
  QList<InputEdit *> l =
      findChildren<InputEdit *>(QString(), Qt::FindChildrenRecursively);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      InputEdit *w = l.at(i);
      if (w != nullptr) {
        connect(w, SIGNAL(sendHasModified(bool)), this,
                SLOT(chieldModified(bool)));
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
  QList<QGroupBox *> groups = findChildren<QGroupBox *>(p_fs, p_fo);
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
      box->setChecked((keys.count() >= 2));
      foreach (QString s, keys) {
        LineEdit *le = box->findChild<LineEdit *>(s, p_fo);
        if (le != nullptr) {
          if (le->isPasswordInput()) {
            setPassword(le, config->value(s).toByteArray());
          } else {
            le->setValue(config->value(s));
          }
          le->setModified(false);
        } else {
          QSpinBox *sp = box->findChild<QSpinBox *>(s, p_fo);
          if (sp != nullptr)
            sp->setValue(config->value(s).toInt());
        }
      }
      config->endGroup();
    }
  }

  initSignalChanged();
}

void ProviderSettings::saveSectionConfig() {
  QList<QGroupBox *> groups = findChildren<QGroupBox *>(p_fs, p_fo);
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
      QList<LineEdit *> lel = box->findChildren<LineEdit *>(p_fs, p_fo);
      if (lel.count() > 0) {
        for (int i = 0; i < lel.count(); i++) {
          LineEdit *le = lel.at(i);
          if (le != nullptr) {
            if (le->isPasswordInput()) {
              config->setValue(le->objectName(), passwordToBase64(le));
            } else {
              config->setValue(le->objectName(), le->value());
            }
            le->setModified(false);
          }
        }
      }
      QList<QSpinBox *> sbl = box->findChildren<QSpinBox *>(p_fs, p_fo);
      if (sbl.count() > 0) {
        for (int i = 0; i < sbl.count(); i++) {
          QSpinBox *sb = sbl.at(i);
          if (sb != nullptr)
            config->setValue(sb->objectName(), sb->value());
        }
      }
      config->endGroup();
    }
  }
}
