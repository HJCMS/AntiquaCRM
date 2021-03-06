// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configdialog.h"
#include "myicontheme.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QVBoxLayout>

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent) {
  setObjectName("antiqua_config_dialog");
  setWindowTitle(tr("Configuration") + " [*]");
  setSizeGripEnabled(true);
  setMinimumSize(800, 550);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(5, 0, 5, 0);
  layout->setObjectName("vlayout");

  // BEGIN Area{"Settings Widgets"|"ListWidget"}
  QHBoxLayout *horizontal_layout = new QHBoxLayout();
  horizontal_layout->setObjectName("hlayout");

  QScrollArea *m_scrollarea = new QScrollArea(this);
  m_scrollarea->setObjectName("config_dialog_scrollarea");
  m_scrollarea->setWidgetResizable(true);
  horizontal_layout->addWidget(m_scrollarea, Qt::AlignLeft);

  pages = new QStackedWidget(m_scrollarea);
  pages->setObjectName("config_dialog_statckedwidget");
  m_scrollarea->setWidget(pages);

  m_pageGeneral = new GeneralSettings(pages);
  m_pageGeneral->setObjectName("main_settings");
  m_pageGeneral->setPageTitle(tr("Application"));
  m_pageGeneral->setPageIcon(myIcon("list"));
  pages->addWidget(m_pageGeneral);

  m_pageCompany = new CompanySettings(pages);
  m_pageCompany->setObjectName("company_settings");
  m_pageCompany->setPageTitle(tr("Company"));
  m_pageCompany->setPageIcon(myIcon("edit_group"));
  pages->addWidget(m_pageCompany);

  m_pagePrinting = new PrintSettings(pages);
  m_pagePrinting->setObjectName("printing_settings");
  m_pagePrinting->setPageTitle(tr("Printing"));
  m_pagePrinting->setPageIcon(myIcon("printer"));
  pages->addWidget(m_pagePrinting);

  m_pageDatabase = new PgSQLSettings(pages);
  m_pageDatabase->setObjectName("database_settings");
  m_pageDatabase->setPageTitle(tr("Database"));
  m_pageDatabase->setPageIcon(myIcon("database"));
  pages->addWidget(m_pageDatabase);

  m_pageProviders = new ProviderSettings(pages);
  m_pageProviders->setObjectName("provider_settings");
  m_pageProviders->setPageTitle(tr("Provider"));
  m_pageProviders->setPageIcon(myIcon("autostart"));
  pages->addWidget(m_pageProviders);

  m_listWidget = new QListWidget(this);
  m_listWidget->setObjectName("config_dialog_menue");
  m_listWidget->setResizeMode(QListView::Adjust);
  m_listWidget->setSortingEnabled(false);
  m_listWidget->setMinimumWidth(100);
  m_listWidget->setMaximumWidth(150);
  horizontal_layout->addWidget(m_listWidget, Qt::AlignLeft);
  layout->addLayout(horizontal_layout);
  // END

  QDialogButtonBox *m_btnBox = new QDialogButtonBox(this);
  m_btnBox->setObjectName("config_dialog_buttenbox");
  m_btnBox->setOrientation(Qt::Horizontal);

  QPushButton *btn_save = m_btnBox->addButton(QDialogButtonBox::Save);
  btn_save->setText(tr("&Save"));

  QPushButton *btn_close = m_btnBox->addButton(QDialogButtonBox::Close);
  btn_close->setText(tr("&Quit"));

  connect(m_btnBox, SIGNAL(accepted()), this, SLOT(saveConfig()));
  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(closeDialog()));

  layout->addWidget(m_btnBox);

  m_statusbar = new QStatusBar(this);
  m_statusbar->setObjectName("config_dialog_statusbar");
  m_statusbar->setSizeGripEnabled(false);
  layout->addWidget(m_statusbar);

  setLayout(layout);
}

/**
 * @brief ConfigDialog::createItemSelection
 */
int ConfigDialog::exec() {
  for (int i = 0; i < pages->count(); i++) {
    if (pages->widget(i) != nullptr) {
      SettingsWidget *w = qobject_cast<SettingsWidget *>(pages->widget(i));
      if (w != nullptr) {
        w->loadSectionConfig();
        QListWidgetItem *lwi = new QListWidgetItem(m_listWidget);
        lwi->setText(w->getPageTitle());
        lwi->setIcon(w->getPageIcon());
        m_listWidget->addItem(lwi);
        connect(w, SIGNAL(pageModified(bool)), this,
                SLOT(setWindowModified(bool)));
      }
    }
  }
  connect(m_listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this,
          SLOT(setPage(QListWidgetItem *)));

  return QDialog::exec();
}

void ConfigDialog::setPage(QListWidgetItem *item) {
  int i = (m_listWidget->currentRow());
  if (pages->widget(i) != nullptr) {
    pages->setCurrentIndex(i);
  }
  statusMessage(tr("Page %1 entered").arg(i));
}

void ConfigDialog::statusMessage(const QString &message) {
  m_statusbar->showMessage(message, 5000);
}

void ConfigDialog::saveConfig() {
  for (int i = 0; i < m_listWidget->count(); i++) {
    if (pages->widget(i) != nullptr) {
      SettingsWidget *w = qobject_cast<SettingsWidget *>(pages->widget(i));
      if (w != nullptr)
        w->saveSectionConfig();
    }
  }
  m_statusbar->showMessage(tr("Configuration saved successfully"), 3000);
  setWindowModified(false);
}

void ConfigDialog::closeDialog() {
  if (isWindowModified()) {
    m_statusbar->showMessage(tr("Unsafed changes!"));
    return;
  }
  accept();
}
