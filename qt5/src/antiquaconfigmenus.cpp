// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquaconfigmenus.h"
#include "companydata.h"
#include "configdialog.h"
#include "deliverservicedialog.h"
#include "designationsdialog.h"
#include "keywordedit.h"
#include "messagetemplates.h"
#include "publishersedit.h"
#include "storageeditdialog.h"

AntiquaConfigMenus::AntiquaConfigMenus(QMenuBar *parent) : QMenu{parent} {
  setObjectName("configurations_menue");
  setTitle(tr("Configuration"));
  QIcon configIcon(":/icons/configure.png");

  m_tables = addMenu(tr("Database editors"));
  m_tables->setIcon(configIcon);

  ac_company = m_tables->addAction(tr("Company"));
  ac_company->setIcon(configIcon);
  connect(ac_company, SIGNAL(triggered()), SLOT(openCompanyDialog()));

  ac_keywords = m_tables->addAction(tr("Keywords"));
  ac_keywords->setIcon(configIcon);
  connect(ac_keywords, SIGNAL(triggered()), SLOT(openKeywordsDialog()));

  ac_designations = m_tables->addAction(tr("Book cover description"));
  ac_designations->setIcon(configIcon);
  connect(ac_designations, SIGNAL(triggered()), SLOT(openDesignationsDialog()));

  ac_publishers = m_tables->addAction(tr("Book Publishers"));
  ac_publishers->setIcon(configIcon);
  connect(ac_publishers, SIGNAL(triggered()), SLOT(openBookPublisherDialog()));

  ac_storage = m_tables->addAction(tr("Storage"));
  ac_storage->setIcon(configIcon);
  connect(ac_storage, SIGNAL(triggered()), SLOT(openStorageDialog()));

  ac_templates = m_tables->addAction(tr("Templates"));
  ac_templates->setIcon(configIcon);
  connect(ac_templates, SIGNAL(triggered()), SLOT(openTemplatesDialog()));

  ac_delivery = m_tables->addAction(tr("Package Delivery"));
  ac_delivery->setIcon(configIcon);
  connect(ac_delivery, SIGNAL(triggered()), SLOT(openDeliveryDialog()));

  ac_system = addAction(tr("Application Settings"));
  ac_system->setIcon(configIcon);
  connect(ac_system, SIGNAL(triggered()), SLOT(openConfigDialog()));
}

void AntiquaConfigMenus::openConfigDialog() {
  ConfigDialog *d = new ConfigDialog(this);
  d->exec();
  d->deleteLater();
}

void AntiquaConfigMenus::openCompanyDialog() {
  CompanyData *d = new CompanyData(this);
  d->exec();
  d->deleteLater();
}

void AntiquaConfigMenus::openKeywordsDialog() {
  KeywordEdit *d = new KeywordEdit(this);
  d->exec();
  d->deleteLater();
}

void AntiquaConfigMenus::openDesignationsDialog() {
  DesignationsDialog *d = new DesignationsDialog(this);
  d->exec();
  d->deleteLater();
}

void AntiquaConfigMenus::openStorageDialog() {
  StorageEditDialog *d = new StorageEditDialog(this);
  d->exec();
  d->deleteLater();
}

void AntiquaConfigMenus::openDeliveryDialog() {
  DeliverServiceDialog *d = new DeliverServiceDialog(this);
  d->exec();
  d->deleteLater();
}

void AntiquaConfigMenus::openTemplatesDialog() {
  MessageTemplates *d = new MessageTemplates(this);
  d->exec();
  d->deleteLater();
}

void AntiquaConfigMenus::openBookPublisherDialog() {
  PublishersEdit *d = new PublishersEdit(this);
  d->exec();
  d->deleteLater();
}
