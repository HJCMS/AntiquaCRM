// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "toolbar.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QSpacerItem>
#include <QStandardPaths>

static const QString getTarget(QStandardPaths::StandardLocation l) {
  QStringList list = QStandardPaths::standardLocations(l);
  return list.first();
}

ToolBar::ToolBar(QWidget *parent) : QWidget{parent} {
  setObjectName("ImageDialogToolbar");
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

  QHBoxLayout *layout = new QHBoxLayout(this);
  m_setTargets = new QComboBox(this);
  int i = 0;
  m_setTargets->insertItem(i++, tr("Home"),
                           getTarget(QStandardPaths::HomeLocation));
  m_setTargets->insertItem(i++, tr("Desktop"),
                           getTarget(QStandardPaths::DesktopLocation));
  m_setTargets->insertItem(i++, tr("Pictures"),
                           getTarget(QStandardPaths::PicturesLocation));
  m_setTargets->insertItem(i++, tr("Downloads"),
                           getTarget(QStandardPaths::DownloadLocation));
  m_setTargets->insertItem(i++, tr("Documents"),
                           getTarget(QStandardPaths::DocumentsLocation));
#ifdef Q_OS_UNIX
  m_setTargets->insertItem(i++, tr("Temp"),
                           getTarget(QStandardPaths::TempLocation));
  m_setTargets->insertItem(i++, tr("Cache"),
                           getTarget(QStandardPaths::GenericCacheLocation));
#endif

  foreach (QFileInfo info, QDir::drives()) {
    if (info.isReadable())
    {
      QString name = info.baseName().isEmpty() ? "System" : info.baseName();
      m_setTargets->insertItem(i++, name, info.absolutePath());
    }
  }

  QSettings cfg;
  QString p = cfg.value("dirs/images").toString();
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
