// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageviewtoolbar.h"
#include "abstractinput.h"

#include <QActionGroup>
#include <QLayout>
#include <QMenu>
#include <QStandardPaths>
#include <QToolBar>

namespace AntiquaCRM {

ImageViewToolBar::ImageViewToolBar(QWidget *parent) : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  QToolBar *lbar = new QToolBar(this);
  lbar->setMovable(false);
  lbar->setOrientation(Qt::Horizontal);
  lbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  layout->addWidget(lbar);

  ac_reset = lbar->addAction(tr("Reset"));
  ac_reset->setIcon(AntiquaApplIcon("action-redo"));
  connect(ac_reset, SIGNAL(triggered()), SIGNAL(sendReset()));

  ac_cut = lbar->addAction(tr("Cutting"));
  ac_cut->setIcon(AntiquaApplIcon("action-cut"));
  connect(ac_cut, SIGNAL(triggered()), SIGNAL(sendCutting()));

  ac_scale = lbar->addAction(tr("Scale"));
  ac_scale->setIcon(AntiquaApplIcon("view-scale"));
  connect(ac_scale, SIGNAL(triggered()), SIGNAL(sendAdjust()));

  ac_rotate = lbar->addAction(tr("Rotate"));
  ac_rotate->setIcon(AntiquaApplIcon("view-rotate-right"));
  connect(ac_rotate, SIGNAL(triggered()), SIGNAL(sendRotate()));

  // Stretch
  layout->addStretch(1);

  // Tree Actions
  btn_targets = new QPushButton(tr("Display Directory"), this);
  btn_targets->setIcon(AntiquaApplIcon("inode-directory"));
  btn_targets->setToolTip(tr("Changes the directory for Tree view."));
  layout->addWidget(btn_targets);

  QAction *ac = nullptr;
  QMenu *tm = new QMenu(btn_targets);
  ac = tm->addAction(AntiquaApplIcon("folder-documents"), tr("Archiv"));
  ac->setObjectName("archiv");
  connect(ac, SIGNAL(triggered()), SLOT(prepareTargetChange()));

  ac = tm->addAction(AntiquaApplIcon("folder-image"), tr("Import"));
  ac->setObjectName("import");
  connect(ac, SIGNAL(triggered()), SLOT(prepareTargetChange()));

  ac = tm->addAction(AntiquaApplIcon("folder-downloads"), tr("Downloads"));
  ac->setObjectName("downloads");
  connect(ac, SIGNAL(triggered()), SLOT(prepareTargetChange()));

  ac = tm->addAction(AntiquaApplIcon("folder-pictures"), tr("Pictures"));
  ac->setObjectName("pictures");
  connect(ac, SIGNAL(triggered()), SLOT(prepareTargetChange()));

  btn_targets->setMenu(tm);
  setLayout(layout);
}

void ImageViewToolBar::prepareTargetChange() {
  QString _target;
  ASettings cfg(this);
  QString _id = sender()->objectName();
  if (_id == "archiv") {
    _target = cfg.value("dirs/images", QString()).toString();
  } else if (_id == "import") {
    _target = cfg.value("dirs/import", QString()).toString();
  } else if (_id == "downloads") {
    _target = QStandardPaths::locate(QStandardPaths::DownloadLocation,
                                     tr("Downloads"),
                                     QStandardPaths::LocateDirectory);
  } else if (_id == "pictures") {
    _target =
        QStandardPaths::locate(QStandardPaths::PicturesLocation, tr("Pictures"),
                               QStandardPaths::LocateDirectory);
  }

  if (_target.isEmpty()) {
    _target = QStandardPaths::locate(QStandardPaths::DocumentsLocation,
                                     tr("Documents"),
                                     QStandardPaths::LocateDirectory);
  }

  QDir _d(_target);
  if (_d.exists() && _d.isReadable())
    emit sendChangeTarget(_d);
}

} // namespace AntiquaCRM
