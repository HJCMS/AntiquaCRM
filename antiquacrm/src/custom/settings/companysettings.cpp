// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "companysettings.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "myicontheme.h"

#include <QtWidgets>

CompanySettings::CompanySettings(QWidget *parent) : SettingsWidget{parent} {
  setObjectName("company_settings");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("printer_config_layout");

  QString info_txt =
      tr("On this page you configure your company's printer settings.");
  QLabel *info_label = new QLabel(info_txt, this);
  layout->addWidget(info_label);

  QString cp_txt = tr("Company name displayed in the letterhead.");
  QLabel *info_cp = new QLabel(cp_txt, this);
  layout->addWidget(info_cp);

  m_companyName = new QLineEdit(this);
  m_companyName->setObjectName("name");
  m_companyName->setPlaceholderText(tr("Company title"));
  layout->addWidget(m_companyName);

  m_companyShortName = new QLineEdit(this);
  m_companyShortName->setObjectName("shortname");
  m_companyShortName->setPlaceholderText(tr("Company short title"));
  layout->addWidget(m_companyShortName);

  QString ca_txt = tr("Company address for the letterhead.");
  QLabel *info_ca = new QLabel(ca_txt, this);
  layout->addWidget(info_ca);

  m_companyStreet = new QLineEdit(this);
  m_companyStreet->setObjectName("street");
  m_companyStreet->setPlaceholderText(tr("Street Number"));
  layout->addWidget(m_companyStreet);

  m_companyLocation = new QLineEdit(this);
  m_companyLocation->setObjectName("location");
  m_companyLocation->setPlaceholderText(tr("Postalcode Location / Country"));
  layout->addWidget(m_companyLocation);

  m_employer = new QLineEdit(this);
  m_employer->setObjectName("employer");
  m_employer->setPlaceholderText(tr("employer"));
  layout->addWidget(m_employer);

  m_tax_number = new QLineEdit(this);
  m_tax_number->setObjectName("taxnumber");
  m_tax_number->setPlaceholderText(tr("tax number"));
  layout->addWidget(m_tax_number);

  m_phone = new QLineEdit(this);
  m_phone->setObjectName("phone");
  m_phone->setPlaceholderText(tr("phone number"));
  layout->addWidget(m_phone);

  m_fax = new QLineEdit(this);
  m_fax->setObjectName("fax");
  m_fax->setPlaceholderText(tr("fax number"));
  layout->addWidget(m_fax);

  m_email = new QLineEdit(this);
  m_email->setObjectName("email");
  m_email->setPlaceholderText(tr("email"));
  layout->addWidget(m_email);

  QString remarks_txt = tr("Notes on the order form.");
  QLabel *info_remarks = new QLabel(remarks_txt, this);
  layout->addWidget(info_remarks);

  m_remarks = new TextField(this);
  m_remarks->setObjectName("remarks");
  layout->addWidget(m_remarks);

  layout->addStretch(1);
  setLayout(layout);
}

void CompanySettings::setPageTitle(const QString &title) {
  pageTitle = title;
  emit pageTitleChanged();
}

const QString CompanySettings::getPageTitle() { return pageTitle; }

void CompanySettings::setPageIcon(const QIcon &icon) {
  pageIcon = icon;
  emit pageIconChanged();
}

const QIcon CompanySettings::getPageIcon() { return pageIcon; }

void CompanySettings::loadSectionConfig() {
  config->beginGroup("company");
  QList<QLineEdit *> edits =
      findChildren<QLineEdit *>(QString(), Qt::FindDirectChildrenOnly);
  for (int i = 0; i < edits.count(); i++) {
    QLineEdit *edit = edits.at(i);
    edit->setText(config->value(edit->objectName()).toString());
  }
  m_remarks->setValue(config->value("remarks"));
  config->endGroup();
}

void CompanySettings::saveSectionConfig() {
  config->beginGroup("company");
  QList<QLineEdit *> edits =
      findChildren<QLineEdit *>(QString(), Qt::FindDirectChildrenOnly);
  for (int i = 0; i < edits.count(); i++) {
    QLineEdit *edit = edits.at(i);
    config->setValue(edit->objectName(), edit->text());
  }
  config->setValue("remarks", m_remarks->value());
  config->endGroup();
}
