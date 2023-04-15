// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageviewtoolbar.h"
#include "abstractinput.h"

#include <QHBoxLayout>
#include <QMenu>
#include <QToolBar>
#include <QtCore>

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
  btn_targets = new QPushButton(tr("Open Directory"), this);
  btn_targets->setIcon(AntiquaApplIcon("inode-directory"));
  btn_targets->setToolTip(tr("Open directory for Tree view."));
  layout->addWidget(btn_targets);

  QAction *ac = nullptr;
  QMenu *tm = new QMenu(btn_targets);
  ac = tm->addAction(AntiquaApplIcon("folder-documents"), tr("Documents"));
  ac->setObjectName("documents");
  connect(ac, SIGNAL(triggered()), SLOT(prepareTargetChange()));

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

#ifdef Q_OS_LINUX
  ac = tm->addAction(AntiquaApplIcon("folder-public"), tr("Public"));
  ac->setObjectName("public");
  connect(ac, SIGNAL(triggered()), SLOT(prepareTargetChange()));
#endif

  btn_targets->setMenu(tm);
  setLayout(layout);
}

const QString ImageViewToolBar::xdgUserDir(const QString &search) const {
  QString _out;
  QString _home = QDir::homePath();
  QFileInfo _file(_home + "/.config/user-dirs.dirs");
  if (_file.isReadable()) {
    QFile _fp(_file.filePath());
    if (!_fp.open(QIODevice::ReadOnly))
      return _out;

    QTextStream _data(&_fp);
    while (!_data.atEnd()) {
      QString _line = _data.readLine();
      if (_line.contains(search)) {
        QString _var = _line.split("=").last();
        _var.replace("\"", "");
        _var.replace("\\", "");
        _var.replace("$HOME", _home);
        _file.setFile(_var.trimmed());
        if (_file.exists())
          _out = _file.filePath();
      }
    }
    _fp.close();
  }
  return _out;
}

void ImageViewToolBar::prepareTargetChange() {
  QString _dest;
  ASettings _cfg(this);
  QString _id = sender()->objectName();
  if (_id == "archiv") {
    _dest = _cfg.value("dirs/images", QString()).toString();
  } else if (_id == "import") {
    _dest = _cfg.value("dirs/import", QString()).toString();
  } else if (_id == "downloads") {
    _dest = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
  } else if (_id == "pictures") {
    _dest = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
  } else if (_id == "documents") {
    _dest = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
  }
#ifdef Q_OS_LINUX
  else if (_id == "public") {
    _dest = xdgUserDir("XDG_PUBLICSHARE_DIR");
  }
#endif

  if (_dest.isEmpty()) {
    _dest = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  }

  QDir _d(_dest);
  if (_d.exists() && _d.isReadable())
    emit sendChangeTarget(_d);
}

} // namespace AntiquaCRM
