// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageviewtoolbar.h"

#include <ASettings>
#include <AntiquaInput>
#include <QDialog>
#include <QDirIterator>
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

  ac_cut = lbar->addAction(tr("Cutting"));
  ac_cut->setIcon(AntiquaApplIcon("action-cut"));
  ac_cut->setToolTip(tr("Cut image to Rubberband."));
  ac_cut->setStatusTip(ac_cut->toolTip());
  connect(ac_cut, SIGNAL(triggered()), SIGNAL(sendCutting()));

  ac_scale = lbar->addAction(tr("Scale"));
  ac_scale->setIcon(AntiquaApplIcon("view-scale"));
  ac_scale->setToolTip(tr("Scale image to view."));
  ac_scale->setStatusTip(ac_scale->toolTip());
  connect(ac_scale, SIGNAL(triggered()), SIGNAL(sendAdjust()));

  ac_rotate = lbar->addAction(tr("Rotate"));
  ac_rotate->setIcon(AntiquaApplIcon("view-rotate-right"));
  ac_rotate->setToolTip(tr("Rotate image clockwise."));
  ac_rotate->setStatusTip(ac_rotate->toolTip());
  connect(ac_rotate, SIGNAL(triggered()), SIGNAL(sendRotate()));

  lbar->addSeparator();

  ac_reset = lbar->addAction(tr("Reset to default"));
  ac_reset->setIcon(AntiquaApplIcon("action-redo"));
  ac_reset->setToolTip(tr("Revert the view back to the original."));
  ac_reset->setStatusTip(ac_reset->toolTip());
  connect(ac_reset, SIGNAL(triggered()), SIGNAL(sendReset()));

  // Stretch
  layout->addStretch(1);

  // Search
  QPushButton *ac_search = new QPushButton(tr("Search"), lbar);
  ac_search->setIcon(AntiquaApplIcon("action-search"));
  ac_search->setToolTip(tr("Search picture with article number."));
  ac_search->setStatusTip(ac_search->toolTip());
  connect(ac_search, SIGNAL(clicked()), SLOT(searchArticleImage()));
  layout->addWidget(ac_search);

  // Tree Actions
  btn_targets = new QPushButton(tr("Open Directory"), this);
  btn_targets->setIcon(AntiquaApplIcon("inode-directory"));
  btn_targets->setToolTip(tr("Open a directory in the tree view."));
  btn_targets->setStatusTip(btn_targets->toolTip());
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

void ImageViewToolBar::searchArticleImage() {
  QDialog *m_dialog = new QDialog(this, Qt::Tool);
  m_dialog->setContentsMargins(0, 0, 0, 0);
  m_dialog->setWindowTitle(tr("Search Image"));
  m_dialog->setMinimumWidth(350);
  m_dialog->setToolTip(tr("Insert Id and press <enter>."));

  QHBoxLayout *layout = new QHBoxLayout(m_dialog);
  layout->setContentsMargins(0, 0, 0, 0);
  ALabel *m_label = new ALabel(tr("Search"), m_dialog);
  m_label->setToolTip(tr("Search Files recursively."));
  layout->addWidget(m_label);
  ALineEdit *m_search = new ALineEdit(m_dialog);
  m_search->setObjectName("search_image");
  m_search->setPlaceholderText(tr("Article number"));
  m_search->setValidation(ALineEdit::InputValidator::NUMERIC);
  layout->addWidget(m_search);
  connect(m_search, SIGNAL(returnPressed()), m_dialog, SLOT(accept()));
  m_dialog->setLayout(layout);
  m_dialog->exec();

  QString _file = m_search->getArticleNumber();
  if (_file.isEmpty())
    return;

  ASettings cfg(this);
  QString _ext = cfg.value("image/extension", "jpg").toString();
  _file.append(".");
  _file.append(_ext);
  _file.prepend(QDir::separator());

  QString _path = cfg.getArchivPath(ANTIQUACRM_ARCHIVE_IMAGES).path();
  _path.append(QDir::separator());
  _path.append("Sources");
  _path.append(QDir::separator());
  _path.append(QDate::currentDate().toString("yyyy"));

  QDir _d(_path);
  if (_d.isReadable()) {
    QDir::Filters filter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Writable);
    QDirIterator it(_d.path(), QStringList(), filter,
                    QDirIterator::Subdirectories);

    QFileInfo _find;
    while (it.hasNext()) {
      _find.setFile(it.next() + _file);
      if (_find.isReadable())
        emit sendSelectArticle(_find.filePath());
    }
  }

  m_dialog->deleteLater();
}

void ImageViewToolBar::prepareTargetChange() {
  QString _dest;
  ASettings _cfg(this);
  QString _id = sender()->objectName();
  if (_id == "archiv") {
    _dest = _cfg.getArchivPath(ANTIQUACRM_ARCHIVE_IMAGES).path();
  } else if (_id == "import") {
    _dest = _cfg.getArchivPath(ANTIQUACRM_ARCHIVE_IMPORT).path();
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
