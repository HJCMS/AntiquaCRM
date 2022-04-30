// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "toolbar.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

ToolBar::ToolBar(QWidget *parent) : QWidget{parent} {
  setObjectName("ImageDialogToolbar");
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  QHBoxLayout *layout = new QHBoxLayout(this);
  m_setTargets = new QComboBox(this);
  int i = 0;
  m_setTargets->insertItem(
      i++, tr("Home"),
      QStandardPaths::standardLocations(QStandardPaths::HomeLocation));
  m_setTargets->insertItem(
      i++, tr("Desktop"),
      QStandardPaths::standardLocations(QStandardPaths::DesktopLocation));
  m_setTargets->insertItem(
      i++, tr("Pictures"),
      QStandardPaths::standardLocations(QStandardPaths::PicturesLocation));
  m_setTargets->insertItem(
      i++, tr("Downloads"),
      QStandardPaths::standardLocations(QStandardPaths::DownloadLocation));
  m_setTargets->insertItem(
      i++, tr("Documents"),
      QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation));
#ifdef Q_OS_UNIX
  m_setTargets->insertItem(
      i++, tr("Temp"),
      QStandardPaths::standardLocations(QStandardPaths::TempLocation));
  m_setTargets->insertItem(
      i++, tr("Cache"),
      QStandardPaths::standardLocations(QStandardPaths::GenericCacheLocation));
#endif

  QSettings cfg;
  QString p = cfg.value("imaging/sourcepath").toString();
  if (!p.isEmpty()) {
    m_setTargets->insertItem(i++, tr("Company Storage"), p);
  }
  layout->addWidget(m_setTargets);

  QSpacerItem *sp =
      new QSpacerItem(8, 8, QSizePolicy::Expanding, QSizePolicy::Minimum);
  layout->addSpacerItem(sp);

  QLabel *lb = new QLabel(this);
  lb->setText(tr("Supported Format: JPG"));
  layout->addWidget(lb);

  layout->addSpacerItem(sp);

  QPushButton *m_up = new QPushButton(tr("Upward"), this);
  m_up->setToolTip(tr("Upward"));
  m_up->setIcon(myIcon("undo"));
  layout->addWidget(m_up);

  setLayout(layout);

  connect(m_up, SIGNAL(clicked(bool)), this, SLOT(goUpClicked(bool)));

  connect(m_setTargets, SIGNAL(currentIndexChanged(int)), this,
          SLOT(targetChanged(int)));
}

void ToolBar::targetChanged(int index) {
  emit goTo(m_setTargets->itemData(index, Qt::UserRole).toString());
}

void ToolBar::goUpClicked(bool) { emit goUp(); }
