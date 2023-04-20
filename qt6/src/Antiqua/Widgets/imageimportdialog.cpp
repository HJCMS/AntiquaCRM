// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageimportdialog.h"
#include "imagefilesource.h"
#include "imageviewer.h"
#include "private/imagetreepathview.h"
#include "private/imagetreeview.h"
#include "private/imageviewtoolbar.h"
#include "splitter.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QFrame>
#include <QImageReader>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusTipEvent>

namespace AntiquaCRM {

ImageImportDialog::ImageImportDialog(int articleId, const QString &category,
                                     QWidget *parent)
    : QDialog{parent}, p_article_id{articleId}, p_category{category} {
  setObjectName("image_import_dialog");
  setWindowTitle(tr("Import Edit Source Images") + "[*]");
  setWindowIcon(AntiquaApplIcon("antiquacrm"));
  setMinimumSize(700, 500);
  setSizeGripEnabled(true);
  setContentsMargins(2, 2, 2, 0);

  config = new ASettings(this);

  // Starte layout
  layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
  layout->setContentsMargins(0, 0, 0, 0);

  // Splitter
  splitter = new AntiquaCRM::Splitter(this);

  // ImageViewer
  viewer = new ImageViewer(this);
  viewer->installEventFilter(this);
  splitter->addLeft(viewer);

  // ImageTreeView
  QFrame *rightFrame = new QFrame(this);
  QBoxLayout *rLayout = new QBoxLayout(QBoxLayout::TopToBottom, rightFrame);
  rLayout->setContentsMargins(0, 0, 0, 0);
  pathView = new ImageTreePathView(rightFrame);
  rLayout->addWidget(pathView);

  treeView = new ImageTreeView(rightFrame);
  rLayout->addWidget(treeView, 1);

  rightFrame->setLayout(rLayout);
  splitter->addRight(rightFrame);
  layout->addWidget(splitter, 1);

  // ImageToolBar
  toolBar = new ImageViewToolBar(this);
  toolBar->setEnabled(false);
  layout->addWidget(toolBar);

  // ButtonBox
  buttonBox = new QDialogButtonBox(this);
  QPushButton *btn_save = buttonBox->addButton(QDialogButtonBox::Save);
  btn_save->setIcon(AntiquaApplIcon("action-save"));
  btn_save->setToolTip(tr("Save image to database."));
  btn_save->setStatusTip(btn_save->toolTip());
  QPushButton *btn_close = buttonBox->addButton(QDialogButtonBox::Close);
  btn_close->setIcon(AntiquaApplIcon("action-quit"));
  btn_close->setToolTip(tr("End dialog and process data."));
  btn_close->setStatusTip(btn_close->toolTip());
  layout->addWidget(buttonBox);

  // StatusBox
  statusBar = new QStatusBar(this);
  statusBar->setSizeGripEnabled(false);
  layout->addWidget(statusBar);

  setLayout(layout);

  // Signals::ImageTreeView
  connect(treeView, SIGNAL(sendSelected(const QFileInfo &)),
          SLOT(imageSelected(const QFileInfo &)));

  connect(treeView, SIGNAL(sendPathChanged(const QDir &)), pathView,
          SLOT(setDirectory(const QDir &)));

  // Signals::ImageTreePathView
  connect(pathView, SIGNAL(sendSelected(const QFileInfo &)),
          SLOT(imageSelected(const QFileInfo &)));

  // Signals::ImageViewToolBar
  connect(toolBar, SIGNAL(sendReset()), viewer, SLOT(reset()));
  connect(toolBar, SIGNAL(sendCutting()), viewer, SLOT(cutting()));
  connect(toolBar, SIGNAL(sendRotate()), viewer, SLOT(rotate()));
  connect(toolBar, SIGNAL(sendAdjust()), viewer, SLOT(adjust()));
  connect(toolBar, SIGNAL(sendChangeTarget(const QDir &)), treeView,
          SLOT(setChangeRoot(const QDir &)));
  connect(toolBar, SIGNAL(sendSelectArticle(const QString &)), treeView,
          SLOT(setShowSource(const QString &)));

  // Signals::QDialogButtonBox
  connect(btn_save, SIGNAL(clicked()), SLOT(aboutToSave()));
  connect(btn_close, SIGNAL(clicked()), SLOT(aboutToQuit()));
}

ImageImportDialog::~ImageImportDialog() {
  // do not destroy „source“ here!
  config->deleteLater();
  viewer->deleteLater();
  treeView->deleteLater();
}

void ImageImportDialog::initialConfiguration() {
  config->beginGroup("dirs");
  p_target = QDir(config->value("images", QString()).toString());
  if (!p_target.exists()) {
    p_import = config->getArchivPath("images");
  }
  p_import = QDir(config->value("import", QString()).toString());
  if (!p_import.exists()) {
    p_import = config->getArchivPath("import");
  }
  config->endGroup();

  if (!p_import.exists() || !p_target.exists()) {
    QMessageBox::warning(this, tr("Configurtion Error"),
                         tr("Can not find Import Directory. Please Check your "
                            "Archive settings."));
  }

  if (!p_target.exists(p_category))
    p_target.mkdir(p_category);

  QString _path = p_target.path();
  _path.append(QDir::separator());
  _path.append(p_category);
  p_destination.setPath(_path);

  config->beginGroup("dialog/imaging");
  if (config->contains("geometry"))
    restoreGeometry(config->value("geometry").toByteArray());
  config->endGroup();
}

ImageFileSource *ImageImportDialog::findSource(QDir dir, qint64 id) {
  ImageFileSource *_ifs = new ImageFileSource(p_target.path());
  _ifs->setFileId(id);

  if (dir.exists()) {
    QStringList _found;
    QString _simple = QString::number(id);
    QString _normal = _ifs->toBaseName(id);
    QStringList _format;
    _format << _simple + ".JPG";
    _format << _normal + ".JPG";
    _format << _simple + ".jpg";
    _format << _normal + ".jpg";

    QDirIterator it(dir.path(), _format, (QDir::Files | QDir::Readable),
                    QDirIterator::Subdirectories);
    while (it.hasNext()) {
      QFileInfo _info(it.next());
      _found << _info.filePath();
    }
    if (_found.size() > 0) {
      _ifs->setSource(_found.first());
      _ifs->setDestination(p_destination.path());
    }
  }
  return _ifs;
}

void ImageImportDialog::closeEvent(QCloseEvent *e) {
  if (e->type() == QEvent::Close) {
    e->setAccepted(false);
    statusBar->showMessage(tr("Please use Dialog Buttons to safely quit!"));
    return;
  }
  QDialog::closeEvent(e);
}

bool ImageImportDialog::event(QEvent *e) {
  if (e->type() == QEvent::StatusTip) {
    QStatusTipEvent *t = static_cast<QStatusTipEvent *>(e);
    if (t->tip().isEmpty())
      return false;

    statusBar->showMessage(t->tip(), 1000);
    return true;
  }
  return QDialog::event(e);
}

bool ImageImportDialog::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::ModifiedChange) {
    QWidget *w = qobject_cast<QWidget *>(obj);
    if (w != nullptr && w->isWindowModified()) {
      setWindowModified(true);
      return true;
    }
  }
  return QObject::eventFilter(obj, event);
}

void ImageImportDialog::setViewerImage(const QString &path) {
  QImageReader reader(path);
  QImage _image = reader.read();
  if (_image.isNull())
    return;

  viewer->setImage(_image);
}

void ImageImportDialog::imageSelected(const QFileInfo &src) {
  if (!src.isReadable())
    return;

  setViewerImage(src.filePath());
  source->setSource(src.filePath());
  source->setPixmap(viewer->getPixmap());
  toolBar->setEnabled(true);
  setWindowModified(!source->compare());
}

void ImageImportDialog::aboutToSave() {
  if (p_article_id < 1) {
    statusBar->showMessage(tr("Invalid Article number - abort!"));
    return;
  }

  if (!source->isValid()) {
    statusBar->showMessage(tr("No valid Data found!"));
    return;
  }

  source->setPixmap(viewer->getPixmap());
  if (source->getPixmap().isNull()) {
    statusBar->showMessage(tr("No Pixmap to save - abort!"));
    return;
  }

  AntiquaCRM::ASqlCore m_sql(this);
  if (!source->storeInDatabase(&m_sql, p_article_id)) {
    statusBar->showMessage(tr("Database error occurred!"));
    return;
  }

  QPixmap _thumbnail = source->getThumbnail();
  if (!_thumbnail.isNull())
    emit sendThumbnail(_thumbnail);

  statusBar->showMessage(tr("Start Image source file operations!"));
  QString _filename(source->toBaseName(p_article_id));
  _filename.append(".jpg");

  QPixmap _pixmap = source->getPixmap();
  QImage _image = _pixmap.toImage();
  QFileInfo ouput(p_destination, _filename);
  QFile fp(ouput.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    _image.save(&fp, "jpeg", 90);
    fp.close();
    setWindowModified(false);
    source->setSource(ouput.filePath());
    statusBar->showMessage(tr("Saved"));
  }

  // Shutdown
  config->beginGroup("dialog/imaging");
  config->setValue("geometry", saveGeometry());
  config->endGroup();
}

void ImageImportDialog::aboutToQuit() {
  if (!isWindowModified()) {
    done(QDialog::Accepted);
    return;
  }

  int _retval = QMessageBox::question(
      this, tr("Unsaved changes"),
      tr("<b>You have unsaved changes.</b><p>Do you really want to close this "
         "Dialog?</p><b>Notes:</b><br>When saving, the Database Thumbnail will "
         "replaced.<p>The Server upload or replace the Source file to your "
         "Online Storage.</p><p>If you not want to replace Images, click "
         "Yes.</p>"));
  if (_retval == QMessageBox::Yes)
    done(QDialog::Rejected);
}

ImageFileSource *ImageImportDialog::currentSource() { return source; }

int ImageImportDialog::exec() {
  if (p_article_id < 1) {
    qWarning("ImageEditor: Article number not set!");
    return QDialog::Rejected;
  }

  initialConfiguration();

  if (!p_target.exists()) {
    qWarning("ImageEditor: Configuration mismatch!");
    return QDialog::Rejected;
  }
  // check from category folder
  source = findSource(p_destination, p_article_id);
  if (!source->isValid()) {
    QString _path = p_target.path();
    _path.append(QDir::separator());
    _path.append("Sources");
    source = findSource(QDir(_path), p_article_id);
  }
  // check again ...
  if (source->isValid()) {
    treeView->setDirectory(p_target);
    treeView->setShowSource(source->getSourcePath());
  } else {
    // fallback default selecters directory
    statusBar->showMessage(tr("No stored Sources by Article number found!"));
    treeView->setDirectory(p_import);
  }

  return QDialog::exec();
}

} // namespace AntiquaCRM
