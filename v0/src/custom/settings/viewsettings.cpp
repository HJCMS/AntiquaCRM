// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "viewsettings.h"
#include "myicontheme.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFontDialog>

ViewSettings::ViewSettings(QWidget *parent) : SettingsWidget{parent} {
  setObjectName("view_settings");
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("general_config_layout");

  // BEGIN Schriften Darstellung
  QGridLayout *gridLayout = new QGridLayout();
  view_font_config = new QLabel(this);
  view_font_config->setText(tr("fonts representation"));
  gridLayout->addWidget(view_font_config, 0, 0, 1, 1, Qt::AlignRight);

  QPushButton *btn_fontdialog = new QPushButton(tr("Open"), this);
  btn_fontdialog->setIcon(myIcon("toggle_log"));
  gridLayout->addWidget(btn_fontdialog, 0, 1, 1, 1, Qt::AlignLeft);
  // END

  layout->addLayout(gridLayout);
  layout->addStretch(1);
  setLayout(layout);

  connect(btn_fontdialog, SIGNAL(clicked()), this, SLOT(openFontDialog()));
}

void ViewSettings::initSignalChanged() { /* TODO */
}

void ViewSettings::openFontDialog() {
  bool b = true;
  QFont font = QFontDialog::getFont(&b, qApp->font(), this);
  if (b) {
    view_font_config->setFont(font);
    chieldModified(true);
  }
}

void ViewSettings::setPageTitle(const QString &title) {
  pageTitle = title;
  emit pageTitleChanged();
}

const QString ViewSettings::getPageTitle() { return pageTitle; }

void ViewSettings::setPageIcon(const QIcon &icon) {
  pageIcon = icon;
  emit pageIconChanged();
}

const QIcon ViewSettings::getPageIcon() { return pageIcon; }

void ViewSettings::loadSectionConfig() {
  config->beginGroup("application");
  QFont font = qApp->font();
  if (!font.fromString(config->value("font").toString())) {
    font.setFamily(config->value("font_family").toString());
    font.setPointSize(config->value("font_size", font.pointSize()).toInt());
  }
  view_font_config->setFont(font);
  config->endGroup();
}

void ViewSettings::saveSectionConfig() {
  QFont font = view_font_config->font();
  config->beginGroup("application");
  config->setValue("font", font.toString());
  config->setValue("font_size", font.pointSize());
  config->setValue("font_family", font.family());
  config->endGroup();
}
