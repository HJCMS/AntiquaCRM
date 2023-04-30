// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "pathsettingsgroup.h"

#include <QGroupBox>
#include <QLayout>

PathSettingsGroup::PathSettingsGroup(QWidget *parent) : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  // Archive Paths
  QGroupBox *m_group1 = new QGroupBox(this);
  m_group1->setTitle(tr("Archive paths"));
  // m_group1->setStyleSheet("QFrame {border: 1px solid red;}");

  QVBoxLayout *grLayout1 = new QVBoxLayout(m_group1);
  grLayout1->setContentsMargins(5, 5, 5, 5);

  m_setDelivery = new AntiquaCRM::SelectTargets(m_group1);
  m_setDelivery->setObjectName(configPath(ANTIQUACRM_ARCHIVE_DELIVERY));
  m_setDelivery->setToolTip(tr("delivery notes"));
  m_setDelivery->setBuddyLabel(tr("Delivery"));
  grLayout1->addWidget(m_setDelivery);

  m_setInvoices = new AntiquaCRM::SelectTargets(m_group1);
  m_setInvoices->setObjectName(configPath(ANTIQUACRM_ARCHIVE_INVOICES));
  m_setInvoices->setToolTip(tr("invoice printouts"));
  m_setInvoices->setBuddyLabel(tr("Invoice"));
  grLayout1->addWidget(m_setInvoices);

  m_setReminder = new AntiquaCRM::SelectTargets(m_group1);
  m_setReminder->setObjectName(configPath(ANTIQUACRM_ARCHIVE_REMINDERS));
  m_setReminder->setToolTip(tr("payment reminders"));
  m_setReminder->setBuddyLabel(tr("Reminders"));
  grLayout1->addWidget(m_setReminder);

  m_setReports = new AntiquaCRM::SelectTargets(m_group1);
  m_setReports->setObjectName(configPath(ANTIQUACRM_ARCHIVE_REPORTS));
  m_setReports->setToolTip(tr("monthly reports"));
  m_setReports->setBuddyLabel(tr("Reports"));
  grLayout1->addWidget(m_setReports);

  m_setCards = new AntiquaCRM::SelectTargets(m_group1);
  m_setCards->setObjectName(configPath(ANTIQUACRM_ARCHIVE_CARDS));
  m_setCards->setToolTip(tr("book cards"));
  m_setCards->setBuddyLabel(tr("Cards"));
  grLayout1->addWidget(m_setCards);

  m_setImageArchive = new AntiquaCRM::SelectTargets(m_group1);
  m_setImageArchive->setObjectName(configPath(ANTIQUACRM_ARCHIVE_IMAGES));
  m_setImageArchive->setToolTip(tr("image archive"));
  m_setImageArchive->setBuddyLabel(tr("Images"));
  grLayout1->addWidget(m_setImageArchive);

  m_setImageImport = new AntiquaCRM::SelectTargets(m_group1);
  m_setImageImport->setObjectName(configPath(ANTIQUACRM_ARCHIVE_IMPORT));
  m_setImageImport->setToolTip(tr("image import"));
  m_setImageImport->setBuddyLabel(tr("Import"));
  grLayout1->addWidget(m_setImageImport);

  grLayout1->addStretch(1);
  m_group1->setLayout(grLayout1);
  layout->addWidget(m_group1);

  // Program Paths
  QGroupBox *m_group2 = new QGroupBox(this);
  m_group2->setTitle(tr("Application paths"));
  // m_group2->setStyleSheet("QFrame {border: 1px solid red;}");

  QGridLayout *grLayout2 = new QGridLayout(m_group2);
  grLayout2->setContentsMargins(5, 5, 5, 5);

  // appl_mailler

  // grLayout2->setRowStretch(, 1);
  m_group2->setLayout(grLayout2);
  layout->addWidget(m_group2);

  // final
  setLayout(layout);
}

const QString PathSettingsGroup::configPath(const QString &section) const {
  QString _p(ANTIQUACRM_ARCHIVE_CONFIG_DIRS);
  _p.append("/");
  _p.append(section);
  return _p;
}
