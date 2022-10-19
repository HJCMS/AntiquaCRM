// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquaconfigmenues.h"
#include "configdialog.h"
#include "deliverservicedialog.h"
#include "keywordedit.h"
#include "storageeditdialog.h"

AntiquaConfigMenues::AntiquaConfigMenues(QWidget *parent) : QMenu{parent} {
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

  ac_delivery = m_tables->addAction(tr("Delivery services"));
  ac_delivery->setIcon(configIcon);
  connect(ac_delivery, SIGNAL(triggered()), SLOT(openDeliveryDialog()));

  ac_system = addAction(tr("Application Settings"));
  ac_system->setIcon(configIcon);
  connect(ac_system, SIGNAL(triggered()), SLOT(openConfigDialog()));
}

void AntiquaConfigMenues::openConfigDialog() {
  ConfigDialog *d = new ConfigDialog(this);
  d->exec();
  d->deleteLater();
}

void AntiquaConfigMenues::openKeywordsDialog() {
  KeywordEdit *d = new KeywordEdit(this);
  d->exec();
  d->deleteLater();
}

void AntiquaConfigMenues::openStorageDialog() {
  StorageEditDialog *d = new StorageEditDialog(this);
  d->exec();
  d->deleteLater();
}

void AntiquaConfigMenues::openDeliveryDialog() {
  DeliverServiceDialog *d = new DeliverServiceDialog(this);
  d->exec();
  d->deleteLater();
}
