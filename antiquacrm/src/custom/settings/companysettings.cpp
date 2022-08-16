// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "companysettings.h"

#include <QGridLayout>

CompanySettings::CompanySettings(QWidget *parent) : SettingsWidget{parent} {
  setObjectName("company_settings");

  QGridLayout *layout = new QGridLayout(this);
  layout->setObjectName("printer_config_layout");
  int row = 0;

  QString info_txt =
      tr("On this page you configure your company's presentation settings.");
  QLabel *info_label = new QLabel(info_txt, this);
  layout->addWidget(info_label, row++, 0, 1, 2);

  QString cp_txt = tr("Company name displayed in the letterhead.");
  QLabel *info_cp = new QLabel(cp_txt, this);
  layout->addWidget(info_cp, row++, 0, 1, 2);

  layout->addWidget(infoCell(tr("Company name")), row, 0, 1, 1, Qt::AlignRight);
  m_companyName = new QLineEdit(this);
  m_companyName->setObjectName("name");
  m_companyName->setPlaceholderText(tr("Full Company title"));
  layout->addWidget(m_companyName, row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Company")), row, 0, 1, 1, Qt::AlignRight);
  m_companyShortName = new QLineEdit(this);
  m_companyShortName->setObjectName("shortname");
  m_companyShortName->setPlaceholderText(tr("Company short title"));
  layout->addWidget(m_companyShortName, row++, 1, 1, 1);

  QString ca_txt = tr("Company address for the letterhead.");
  QLabel *info_ca = new QLabel(ca_txt, this);
  layout->addWidget(info_ca, row++, 0, 1, 2);

  layout->addWidget(infoCell(tr("Street")), row, 0, 1, 1, Qt::AlignRight);
  m_companyStreet = new QLineEdit(this);
  m_companyStreet->setObjectName("street");
  m_companyStreet->setPlaceholderText(tr("Street Number"));
  layout->addWidget(m_companyStreet, row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Location")), row, 0, 1, 1, Qt::AlignRight);
  m_companyLocation = new QLineEdit(this);
  m_companyLocation->setObjectName("location");
  m_companyLocation->setPlaceholderText(tr("Postalcode Location / Country"));
  layout->addWidget(m_companyLocation, row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Employer")), row, 0, 1, 1, Qt::AlignRight);
  m_employer = new QLineEdit(this);
  m_employer->setObjectName("employer");
  m_employer->setPlaceholderText(tr("employer"));
  layout->addWidget(m_employer, row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Phone number")), row, 0, 1, 1, Qt::AlignRight);
  m_phone = new QLineEdit(this);
  m_phone->setObjectName("phone");
  m_phone->setPlaceholderText(tr("phone number"));
  layout->addWidget(m_phone, row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Fax Number")), row, 0, 1, 1, Qt::AlignRight);
  m_fax = new QLineEdit(this);
  m_fax->setObjectName("fax");
  m_fax->setPlaceholderText(tr("fax number"));
  layout->addWidget(m_fax, row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("eMail")), row, 0, 1, 1, Qt::AlignRight);
  m_email = new QLineEdit(this);
  m_email->setObjectName("email");
  m_email->setPlaceholderText(tr("email"));
  layout->addWidget(m_email, row++, 1, 1, 1);

  QString banking_txt = tr("Bank details on Document footer bottom right.");
  QLabel *info_banking = new QLabel(banking_txt, this);
  layout->addWidget(info_banking, row++, 0, 1, 2);

  layout->addWidget(infoCell(tr("Bank")), row, 0, 1, 1, Qt::AlignRight);
  m_bankname = new QLineEdit(this);
  m_bankname->setObjectName("bank");
  m_bankname->setPlaceholderText(tr("bankname"));
  layout->addWidget(m_bankname, row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("BIC Swift")), row, 0, 1, 1, Qt::AlignRight);
  m_bicswift = new QLineEdit(this);
  m_bicswift->setObjectName("bicswift");
  m_bicswift->setPlaceholderText(tr("BIC Swift Code"));
  layout->addWidget(m_bicswift, row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("IBAN")), row, 0, 1, 1, Qt::AlignRight);
  m_iban = new QLineEdit(this);
  m_iban->setObjectName("iban");
  m_iban->setPlaceholderText(tr("IBAN number"));
  layout->addWidget(m_iban, row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Tax number")), row, 0, 1, 1, Qt::AlignRight);
  m_tax_number = new QLineEdit(this);
  m_tax_number->setObjectName("taxnumber");
  m_tax_number->setPlaceholderText(tr("Tax number"));
  layout->addWidget(m_tax_number, row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Legality")), row, 0, 1, 1, Qt::AlignRight);
  m_legality = new QLineEdit(this);
  m_legality->setObjectName("legality");
  m_legality->setPlaceholderText(tr("Legality"));
  layout->addWidget(m_legality, row++, 1, 1, 1);

  layout->setRowStretch(row, 1);
  setLayout(layout);
}

QLabel *CompanySettings::infoCell(const QString &txt) {
  QLabel *lb = new QLabel(txt + ":", this);
  return lb;
}

void CompanySettings::lineEditChanged(const QString &) { chieldModified(true); }

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
    connect(edit, SIGNAL(textChanged(const QString &)), this,
            SLOT(lineEditChanged(const QString &)));
  }
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
  config->endGroup();
}
