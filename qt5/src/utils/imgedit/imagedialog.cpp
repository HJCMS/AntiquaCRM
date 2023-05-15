// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagedialog.h"
#include "imageactionsbar.h"
#include "imageselecter.h"
#include "imageview.h"
#include "sourceinfo.h"

#ifndef DEBUG_IMAGE_DIALOG
#define DEBUG_IMAGE_DIALOG true
#endif

#include <QDebug>
#include <QDialogButtonBox>
#include <QDirIterator>
#include <QFileInfo>
#include <QIcon>
#include <QMessageBox>
#include <QStandardPaths>
#include <QVBoxLayout>

ImageDialog::ImageDialog(int articleId, QWidget *parent)
    : QDialog{parent}, p_articleId{articleId} {
  setObjectName("image_open_edit_dialog");
  setWindowTitle(tr("Picture Editor"));
  setMinimumSize(QSize(800, 400));
  setSizeGripEnabled(true);

  config = new AntiquaCRM::ASettings(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("image_open_edit_layout");
  layout->setContentsMargins(10, 0, 0, 0);

  // Left (Image Preview)
  QSize mSize = config->value("image/max_size", QSize(320, 320)).toSize();
  m_splitter = new QSplitter(this);
  m_splitter->setObjectName("image_open_edit_splitter");
  m_view = new ImageView(mSize, m_splitter);
  m_view->setObjectName("image_open_edit_preview");
  m_splitter->insertWidget(0, m_view);
  m_splitter->setStretchFactor(0, 40);

  // Right (Image Selecter)
  m_imageSelecter = new ImageSelecter(m_splitter);
  m_imageSelecter->setObjectName("image_folder_view");
  m_splitter->insertWidget(1, m_imageSelecter);
  m_splitter->setStretchFactor(1, 60);

  // add splitter
  layout->insertWidget(0, m_splitter);

  // add tool/statusbar
  m_statusBar = new QStatusBar(this);
  m_statusBar->setObjectName("image_open_edit_statusbar");
  m_actionsBar = new ImageActionsBar(m_statusBar);
  m_statusBar->addPermanentWidget(m_actionsBar);
  layout->insertWidget(1, m_statusBar);
  layout->setStretch(0, 1);
  setLayout(layout);

  // Signals::ImageSelecter
  connect(m_imageSelecter, SIGNAL(sendSelection(const SourceInfo &)),
          SLOT(fileChanged(const SourceInfo &)));
  connect(m_imageSelecter, SIGNAL(sendImportImage(const QString &)),
          SLOT(setImageImport(const QString &)));
  connect(m_imageSelecter, SIGNAL(sendStatusMessage(const QString &)),
          SLOT(notifyStatus(const QString &)));

  // Signals::ImageView
  connect(m_view, SIGNAL(sendImageLoadSuccess(bool)), m_actionsBar,
          SLOT(enableCustomActions(bool)));

  // Signals::ActionsBars
  connect(m_actionsBar, SIGNAL(sendReset()), m_view, SLOT(reset()));
  connect(m_actionsBar, SIGNAL(sendCutting()), m_view, SLOT(cutting()));
  connect(m_actionsBar, SIGNAL(sendRotate()), m_view, SLOT(rotate()));
  connect(m_actionsBar, SIGNAL(sendAdjust()), m_view, SLOT(adjust()));
  connect(m_actionsBar, SIGNAL(sendSave()), SLOT(save()));
  connect(m_actionsBar, SIGNAL(sendAccept()), SLOT(accept()));
}

bool ImageDialog::findSourceImage() {
  if (!p_archiv.exists()) {
    notifyStatus(tr("archiv isn't readable!"));
    return false;
  }

  const QString _simple = QString::number(p_articleId);
  const QString _long = SourceInfo::imageBaseName(p_articleId);
  QStringList _search;
  _search << _simple + ".JPG";
  _search << _long + ".JPG";
  _search << _simple + ".jpg";
  _search << _long + ".jpg";

  QString _sources(p_archiv.path());
  _sources.append(QDir::separator());
  _sources.append("Sources");

  QMap<QString, QString> _map;
  QDir::Filters filter(QDir::Files | QDir::NoDotAndDotDot);
  foreach (QString d, QStringList({p_savePath.path(), _sources})) {
    QDirIterator it(d, _search, filter, QDirIterator::Subdirectories);
    while (it.hasNext()) {
      QFileInfo f(it.next());
      QDateTime _time = f.fileTime(QFileDevice::FileModificationTime);
      _map.insert(_time.toString("yyyyMMddhhmm"), f.filePath());
    }
  }

  if (_map.size() < 1) {
    return false;
  } else if (_map.size() > 1) {
    qWarning("Found '%d' image files!", _map.size());
  }

  SourceInfo src(p_archiv.path());
  src.setFile(_map.last());
  src.setFileId(p_articleId);
  src.setTarget(p_savePath);
  if (src.isValidSource()) {
    m_imageSelecter->setSelection(src);
    m_view->setImageFile(src);
    _map.clear();
    return true;
  }
  return false;
}

bool ImageDialog::isImageFromArchive(const SourceInfo &info) {
  return (info.path() == p_archiv.path());
}

bool ImageDialog::askToCopyFile() {
  QString question = tr("<p>Do you want to copy this Image into the Picture "
                        "Archiv?</p><b>Note:</b> This will replace Images "
                        "with identical Article Filename!");
  QString title(tr("Copy image?"));
  int ret = QMessageBox::question(this, title, question);
  if (ret == QMessageBox::Yes) {
    return true;
  }
  return false;
}

bool ImageDialog::imagePreview(const SourceInfo &image) {
  SourceInfo info(image);
  info.setFileId(p_articleId);
  info.setTarget(p_savePath);
  if (info.isValidSource()) {
    m_view->setImageFile(info);
    return true;
  }
  return false;
}

void ImageDialog::save() {
  if (m_view->getImage().isNull()) {
    notifyStatus(tr("no valid image found"));
    return;
  }

  SourceInfo info = m_view->getSource();
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << "SourceFileId" << info.getFileId() << Qt::endl
           << "isValid Source:" << info.isValidSource() << Qt::endl
           << "get from Archiv:" << isImageFromArchive(info) << Qt::endl
           << "Destination:" << m_view->getSource().getTarget();
#endif

  if (!info.isValidSource()) {
    qWarning("ImageView::Invalid sourceinfo object");
    return;
  }

  if (isImageFromArchive(info) && askToCopyFile()) {
    // Überschreiben
    notifyStatus(tr("copy image in progress ..."));
    if (m_view->saveImageTo(info)) {
      notifyStatus(tr("Successfully moved Image to archive."));
    } else {
      notifyStatus(tr("Image not moved!"));
      qWarning("image override failed");
    }
  } else {
    if (m_view->saveImageTo(info)) {
      notifyStatus(tr("Successfully copied Image to archive."));
    } else {
      notifyStatus(tr("Image not copied!"));
      qWarning("image copy failed");
    }
  }

  // In Datenbank Speichern!
  if (m_view->storeInDatabase(p_articleId))
    notifyStatus(tr("Image saved successfully!"));
}

void ImageDialog::fileChanged(const SourceInfo &image) {
  if (!imagePreview(image))
    notifyStatus(tr("Image preview rejected!"));
}

void ImageDialog::setImageImport(const QString &image) {
  SourceInfo src(p_savePath.path());
  src.setFileId(p_articleId);
  src.setFile(image);
  if (!imagePreview(src)) {
    notifyStatus(tr("Image preview rejected!"));
  }
}

void ImageDialog::closeEvent(QCloseEvent *e) {
  if (e->type() == QEvent::Close) {
    e->setAccepted(false);
    notifyStatus(tr("Please use the Finish button!"));
    return;
  }
  QDialog::closeEvent(e);
}

void ImageDialog::resizeEvent(QResizeEvent *e) {
  if (e->type() == QEvent::Resize) {
    config->beginGroup("dialog/imaging");
    config->setValue("geometry", saveGeometry());
    config->setValue("windowState", m_splitter->saveState());
    config->endGroup();
  }
  QDialog::resizeEvent(e);
}

void ImageDialog::notifyStatus(const QString &str) {
  m_statusBar->showMessage(str, (10 * 1000));
}

const QDir ImageDialog::getDefaultImagePath() {
  return config->getArchivPath("images");
}

const QDir ImageDialog::getSavePath() {
  // NOTE Muss innerhalb des Archivepfades liegen!
  if (!p_savePath.path().contains(p_archiv.path()))
    p_savePath = p_archiv;

  return p_savePath;
}

void ImageDialog::setSubCategory(const QString &category) {
  p_category = AntiquaCRM::AUtil::ucFirst(category.toLower());
  QDir d = getDefaultImagePath();
  QString _sub = d.path();
  _sub.append(QDir::separator());
  _sub.append(p_category);
  p_savePath = QDir(_sub);
}

int ImageDialog::exec() {
  if (p_articleId < 1) {
    qWarning("No Article number set, image edit aborted!");
    return QDialog::Rejected;
  }

  p_archiv = getDefaultImagePath();
  p_savePath = getSavePath();
  m_imageSelecter->setDirectory(p_archiv.path());

  if (config->contains("dialog/imaging/geometry")) {
    config->beginGroup("dialog/imaging");
    restoreGeometry(config->value("geometry").toByteArray());
    if (config->contains("windowState"))
      m_splitter->restoreState(config->value("windowState").toByteArray());

    config->endGroup();
  }

  if (!findSourceImage() && m_view->readFromDatabase(p_articleId)) {
    notifyStatus(tr("No source image, falling back to database preview!"));
    m_actionsBar->enableCustomActions(false);
  }

  return QDialog::exec();
}
