// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configdialog.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QVBoxLayout>

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent) {
  setObjectName("antiqua_config_dialog");
  setWindowTitle(tr("Configuration"));
  setSizeGripEnabled(true);
  setMinimumSize(800, 560);

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

  ConfigPages = new QStackedWidget(m_scrollarea);
  ConfigPages->setObjectName("config_dialog_statckedwidget");
  m_scrollarea->setWidget(ConfigPages);

  m_page1 = new GeneralSettings(ConfigPages);
  m_page1->setObjectName("main_settings");
  m_page1->setPageTitle(tr("Application"));
  m_page1->setPageIcon(myIcon("list"));
  ConfigPages->addWidget(m_page1);

  m_page2 = new PgSQLSettings(ConfigPages);
  m_page2->setObjectName("database_settings");
  m_page2->setPageTitle(tr("Database"));
  m_page2->setPageIcon(myIcon("database"));
  ConfigPages->addWidget(m_page2);

  m_page3 = new ViewSettings(ConfigPages);
  m_page3->setObjectName("appearance_settings");
  m_page3->setPageTitle(tr("Appearance"));
  m_page3->setPageIcon(myIcon("autostart"));
  ConfigPages->addWidget(m_page3);

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
  for (int i = 0; i < ConfigPages->count(); i++) {
    if (ConfigPages->widget(i) != nullptr) {
      SettingsWidget *w =
          qobject_cast<SettingsWidget *>(ConfigPages->widget(i));
      if (w != nullptr) {
        w->loadSectionConfig();
        QListWidgetItem *lwi = new QListWidgetItem(m_listWidget);
        lwi->setText(w->getPageTitle());
        lwi->setIcon(w->getPageIcon());
        m_listWidget->addItem(lwi);
      }
    }
  }
  connect(m_listWidget, SIGNAL(itemClicked(QListWidgetItem *)), this,
          SLOT(setPage(QListWidgetItem *)));

  return QDialog::exec();
}

void ConfigDialog::setPage(QListWidgetItem *item) {
  int i = (m_listWidget->currentRow());
  if (ConfigPages->widget(i) != nullptr) {
    ConfigPages->setCurrentIndex(i);
  }
  statusMessage(tr("Page %1 entered").arg(i));
}

void ConfigDialog::statusMessage(const QString &message) {
  m_statusbar->showMessage(message, 5000);
}

void ConfigDialog::saveConfig() {
  for (int i = 0; i < m_listWidget->count(); i++) {
    if (ConfigPages->widget(i) != nullptr) {
      SettingsWidget *w =
          qobject_cast<SettingsWidget *>(ConfigPages->widget(i));
      if (w != nullptr)
        w->saveSectionConfig();
    }
  }
  m_statusbar->showMessage(tr("Configuration saved successfully"), 3000);
}

void ConfigDialog::closeDialog() { accept(); }
