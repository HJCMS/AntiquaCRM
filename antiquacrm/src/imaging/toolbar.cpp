// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "toolbar.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>

ToolBar::ToolBar(QWidget *parent) : QToolBar{parent} {
  setObjectName("ImageDialogToolbar");
  setOrientation(Qt::Horizontal);
  setMovable(false);

  QWidget *w = new QWidget(this);
  QHBoxLayout *layout = new QHBoxLayout(w);
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
  layout->addWidget(m_setTargets);

  QSpacerItem *sp = new QSpacerItem(8,8,QSizePolicy::Expanding,QSizePolicy::Minimum);
  layout->addSpacerItem(sp);

  w->setLayout(layout);

  addWidget(w);

  addSeparator();

  QAction *m_up = addAction(myIcon("undo"),tr("Upward"));
  m_up->setToolTip(tr("Upward"));

  connect(m_up, SIGNAL(triggered(bool)), this, SLOT(goUpClicked(bool)));

  connect(m_setTargets, SIGNAL(currentIndexChanged(int)), this,
          SLOT(targetChanged(int)));
}

void ToolBar::targetChanged(int index) {
  emit goTo(m_setTargets->itemData(index, Qt::UserRole).toString());
}

void ToolBar::goUpClicked(bool)
{
    emit goUp();
}
