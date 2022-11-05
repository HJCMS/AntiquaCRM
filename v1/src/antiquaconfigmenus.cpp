// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquaconfigmenus.h"
#include "configdialog.h"
#include "deliverservicedialog.h"
#include "keywordedit.h"
#include "messagetemplates.h"
#include "storageeditdialog.h"

AntiquaConfigMenus::AntiquaConfigMenus(QWidget *parent) : QMenu{parent} {
  setObjectName("configurations_menue");
  setTitle(tr("Configuration"));
  QIcon configIcon(":/icons/configure.png");

  m_tables = addMenu(tr("Sections settings"));
  m_tables->setIcon(configIcon);

  ac_keywords = m_tables->addAction(tr("Keywords Editor"));
  ac_keywords->setIcon(configIcon);
  connect(ac_keywords, SIGNAL(triggered()), SLOT(openKeywordsDialog()));

  ac_storage = m_tables->addAction(tr("Storage Editor"));
  ac_storage->setIcon(configIcon);
  connect(ac_storage, SIGNAL(triggered()), SLOT(openStorageDialog()));

  ac_templates = m_tables->addAction(tr("Templates Editor"));
  ac_templates->setIcon(configIcon);
  connect(ac_templates, SIGNAL(triggered()), SLOT(openTemplatesDialog()));

  ac_delivery = m_tables->addAction(tr("Delivery services"));
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

void AntiquaConfigMenus::openKeywordsDialog() {
  KeywordEdit *d = new KeywordEdit(this);
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
