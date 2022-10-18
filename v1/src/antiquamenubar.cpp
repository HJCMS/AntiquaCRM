// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquamenubar.h"
// Dialogs
#include "configdialog.h"
#include "keywordedit.h"
#include "storageeditdialog.h"

AntiquaMenuBar::AntiquaMenuBar(QMainWindow *parent)
    : QMenuBar{parent}, defaultIcon(QString(":icons/antiqua.png")) {
  setNativeMenuBar(true);
  setStyleSheet("");

  m_applMenu = addMenu("Antiqua CRM");
  addApplMenu();

  m_viewsMenu = addMenu(tr("Views"));
  addViewsMenu();

  m_configMenu = addMenu(tr("Configuration"));
  addConfigMenu();

  m_aboutMenu = addMenu(tr("About"));
  addAboutMenu();
}

void AntiquaMenuBar::addApplMenu() {
  QAction *ac_quit = m_applMenu->addAction(tr("Quit"));
  ac_quit->setIcon(QIcon(":icons/action_quit.png"));
  ac_quit->setShortcut(QKeySequence::Quit);
  connect(ac_quit, SIGNAL(triggered()), this, SIGNAL(sendApplQuit()));
}

void AntiquaMenuBar::addViewsMenu() {
  QAction *ac_views = m_viewsMenu->addAction("TODO");
  ac_views->setIcon(defaultIcon);
}

void AntiquaMenuBar::addConfigMenu() {
  QIcon configIcon(":/icons/configure.png");

  QMenu *m_tables = m_configMenu->addMenu(tr("Tables"));
  m_tables->setIcon(configIcon);

  QAction *ac_keywords = m_tables->addAction(tr("Keywords Editor"));
  ac_keywords->setIcon(configIcon);
  connect(ac_keywords, SIGNAL(triggered()), SLOT(openKeywordsDialog()));

  QAction *ac_storage = m_tables->addAction(tr("Storage Editor"));
  ac_storage->setIcon(configIcon);
  connect(ac_storage, SIGNAL(triggered()), SLOT(openStorageDialog()));

  QAction *ac_config = m_configMenu->addAction(tr("Application Settings"));
  ac_config->setIcon(configIcon);
  connect(ac_config, SIGNAL(triggered()), SLOT(openConfigDialog()));
}

void AntiquaMenuBar::addAboutMenu() {
  QAction *ac_about = m_aboutMenu->addAction("TODO");
  ac_about->setIcon(defaultIcon);
}

void AntiquaMenuBar::openConfigDialog() {
  ConfigDialog *d = new ConfigDialog(this);
  if (d->exec()) {
    qInfo("Configuration done...");
  }
  d->deleteLater();
}

void AntiquaMenuBar::openKeywordsDialog() {
  KeywordEdit *d = new KeywordEdit(this);
  if (d->exec()) {
    qInfo("Keyword edit done...");
  }
  d->deleteLater();
}

void AntiquaMenuBar::openStorageDialog() {
  StorageEditDialog *d = new StorageEditDialog(this);
  if (d->exec()) {
    qInfo("Storage edit done...");
  }
  d->deleteLater();
}
