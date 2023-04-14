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

namespace AntiquaCRM {

ImageImportDialog::ImageImportDialog(int articleId, const QString &category,
                                     QWidget *parent)
    : QDialog{parent}, p_article_id{articleId}, p_category{category} {
  setObjectName("image_import_dialog");
  setWindowTitle(tr("Import Edit Source Images") + "[*]");
  setWindowIcon(AntiquaApplIcon("antiquacrm"));
  setMinimumSize(600, 400);
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
  QPushButton *btn_close = buttonBox->addButton(QDialogButtonBox::Close);
  btn_close->setIcon(AntiquaApplIcon("action-quit"));
  QPushButton *btn_abort = buttonBox->addButton(QDialogButtonBox::Abort);
  btn_abort->setIcon(AntiquaApplIcon("action-quit"));
  layout->addWidget(buttonBox);

  // StatusBox
  statusBar = new QStatusBar(this);
  statusBar->setSizeGripEnabled(false);
  layout->addWidget(statusBar);

  setLayout(layout);

  // Signals::ImageTreeView
  connect(treeView, SIGNAL(sendSelected(const AntiquaCRM::ImageFileSource &)),
          SLOT(imageSelected(const AntiquaCRM::ImageFileSource &)));

  connect(treeView, SIGNAL(sendPathChanged(const QDir &)), pathView,
          SLOT(setDirectory(const QDir &)));

  // Signals::ImageTreePathView
  connect(pathView, SIGNAL(sendSelected(const AntiquaCRM::ImageFileSource &)),
          SLOT(imageSelected(const AntiquaCRM::ImageFileSource &)));

  // Signals::ImageViewer
  connect(viewer, SIGNAL(sendSetSceneView(bool)), toolBar,
          SLOT(setEnabled(bool)));

  // Signals::ImageViewToolBar
  connect(toolBar, SIGNAL(sendReset()), viewer, SLOT(reset()));
  connect(toolBar, SIGNAL(sendCutting()), viewer, SLOT(cutting()));
  connect(toolBar, SIGNAL(sendRotate()), viewer, SLOT(rotate()));
  connect(toolBar, SIGNAL(sendAdjust()), viewer, SLOT(adjust()));
  connect(toolBar, SIGNAL(sendChangeTarget(const QDir &)), treeView,
          SLOT(setChangeRoot(const QDir &)));

  // Signals::QDialogButtonBox
  connect(btn_save, SIGNAL(clicked()), SLOT(aboutToSave()));
  connect(btn_close, SIGNAL(clicked()), SLOT(aboutToQuit()));
  connect(btn_abort, SIGNAL(clicked()), SLOT(reject()));
}

ImageImportDialog::~ImageImportDialog() {
  // do not destroy „source“ here!
  config->deleteLater();
  viewer->deleteLater();
  treeView->deleteLater();
}

bool ImageImportDialog::initialConfiguration() {
  config->beginGroup("dirs");
  p_target = QDir(config->value("images", QString()).toString());
  if (!p_target.exists()) {
    p_import = config->getArchivPath("images");
  }
  bool _a = p_target.exists();

  p_import = QDir(config->value("import", QString()).toString());
  if (!p_import.exists()) {
    p_import = config->getArchivPath("import");
  }
  bool _b = p_import.exists();
  config->endGroup();

  if (!p_target.exists(p_category))
    p_target.mkdir(p_category);

  QString _path = p_target.path();
  _path.append(QDir::separator());
  _path.append(p_category);
  p_store.setPath(_path);

  config->beginGroup("dialog/imaging");
  if (config->contains("geometry"))
    restoreGeometry(config->value("geometry").toByteArray());
  config->endGroup();

  return (_a && _b);
}

ImageFileSource *ImageImportDialog::findSource(QDir inDir, qint64 id) {
  ImageFileSource *_ifs = new ImageFileSource(p_target.path());
  _ifs->setFileId(id);

  if (inDir.exists()) {
    QStringList _found;
    QString s_simple = QString::number(id);
    QString s_normal = _ifs->toBaseName(id);
    QStringList s_filter;
    s_filter << s_simple + ".JPG";
    s_filter << s_normal + ".JPG";
    s_filter << s_simple + ".jpg";
    s_filter << s_normal + ".jpg";
    QDirIterator it(inDir.path(), s_filter, QDir::NoFilter,
                    QDirIterator::Subdirectories);
    while (it.hasNext()) {
      QFileInfo _info(it.next());
      _found << _info.filePath();
    }
    if (_found.size() > 0) {
      _ifs->setFile(_found.first());
      _ifs->setStoreDirectory(p_store.path());
    }
  }
  return _ifs;
}

void ImageImportDialog::closeEvent(QCloseEvent *e) {
  if (e->type() == QEvent::Close) {
    e->setAccepted(false);
    statusBar->showMessage(tr("Use Tool Buttons to safely quit!"));
    return;
  }
  QDialog::closeEvent(e);
}

void ImageImportDialog::setViewerImage(const QString &path) {
  QImageReader reader(path);
  QImage _image = reader.read();
  if (_image.isNull())
    return;

  viewer->setImage(_image);
}

void ImageImportDialog::imageSelected(const AntiquaCRM::ImageFileSource &src) {
  setViewerImage(src.filePath());

  source->setStoreDirectory(p_store.path());
  source->setFile(src.filePath());
  source->setPixmap(viewer->getPixmap());
}

void ImageImportDialog::aboutToSave() {
  if (p_article_id < 1) {
    statusBar->showMessage(tr("Invalid Article number - abort!"));
    return;
  }

  if (!source->isValidSource()) {
    statusBar->showMessage(tr("No valid Data found!"));
    return;
  }

  source->setPixmap(viewer->getPixmap());
  if (source->getPixmap().isNull()) {
    statusBar->showMessage(tr("No Image to save - abort!"));
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
  QFileInfo ouput(p_store, _filename);
  QFile fp(ouput.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    _image.save(&fp, "jpeg", 90);
    fp.close();
    setWindowModified(false);
    source->setFile(ouput.filePath());
    statusBar->showMessage(tr("Saved"));
  }

  // Shutdown
  config->beginGroup("dialog/imaging");
  config->setValue("geometry", saveGeometry());
  config->endGroup();
}

void ImageImportDialog::aboutToQuit() {
  if (isWindowModified()) {
    int _retval =
        QMessageBox::question(this, tr("Unsaved changes"),
                              tr("You have unsaved changes.\n"
                                 "Do you really want to close this Dialog?"));
    if (_retval != QMessageBox::Yes)
      return;
  }
  done(QDialog::Accepted);
}

ImageFileSource *ImageImportDialog::currentSource() { return source; }

int ImageImportDialog::exec() {
  if (p_article_id < 1) {
    qWarning("ImageEditor: Article number not set!");
    return QDialog::Rejected;
  }

  if (!initialConfiguration()) {
    qWarning("ImageEditor: Configuration mismatch!");
    return QDialog::Rejected;
  }

  source = findSource(p_store, p_article_id);
  if (!source->exists()) {
    QString _path = p_target.path();
    _path.append(QDir::separator());
    _path.append("Sources");

    QDir _sourcedir(_path);
    source = findSource(_sourcedir, p_article_id);
  }

  if (source->exists()) {
    treeView->setDirectory(p_target);
    setViewerImage(source->filePath());
    treeView->setShowSource(source->filePath());
  } else {
    // Set Default Selecters directory
    statusBar->showMessage(tr("No stored Sources by Article number found!"));
    treeView->setDirectory(p_import);
  }

  return QDialog::exec();
}

} // namespace AntiquaCRM
