// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagedialog.h"
#include "imaging.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtGui/QPixmap>
#include <QtGui/QImageReader>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

ImageDialog::ImageDialog(qulonglong id, QWidget *parent)
    : QDialog{parent}, imageID(id) {
  setObjectName("ImageDialog");
  setWindowTitle(tr("Picture Editor"));
  setWindowIcon(myIcon("image"));
  setMinimumSize(QSize(600, 400));

  QVBoxLayout *m_layout = new QVBoxLayout(this);

  m_scrollArea = new QScrollArea(this);
  m_scrollArea->setObjectName("dialog_scrollarea");
  m_scrollArea->setVisible(true);
  m_layout->addWidget(m_scrollArea);

  QDialogButtonBox *m_buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
  m_buttonBox->setShortcutEnabled(false);

  m_layout->addWidget(m_buttonBox);

  QPushButton *m_close =
      m_buttonBox->addButton(tr("Close"), QDialogButtonBox::RejectRole);
  m_close->setObjectName("dialog_close_button");
  m_close->setToolTip(tr("Close the dialog without saving."));
  m_close->setIcon(myIcon("exit"));
  m_close->setShortcut(QKeySequence::Close);

  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void ImageDialog::loadFile(const QFileInfo &file) {
  QImageReader reader(file.absoluteFilePath());
  reader.setAutoTransform(true);
  qDebug() << reader.format();
  QImage image = reader.read();
  if (!image.isNull()) {
    QImage medium = image.scaled(QSize(maximumSize,maximumSize),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    if(!medium.isNull())
    {
        m_imageLabel = new Imaging(m_scrollArea);
        m_imageLabel->setPixmap(QPixmap::fromImage(medium));
        qDebug() << "CurrentImageSize" << medium.size();
        m_scrollArea->setWidget(m_imageLabel);
        emit s_imageScaled(true);
        return;
    }
    emit s_imageScaled(false);
  }
}

bool ImageDialog::setSourceTarget(const QString &path) {
  sourceDir.setPath(path);
  if (sourceDir.exists()) {
    QString image(".JPG");
    image.prepend(QString::number(imageID));
    QFileInfo info(sourceDir, image);
    if (info.exists()) {
      loadFile(info);
    }
    return true;
  }
  return false;
}

const QString ImageDialog::sourceTarget() { return sourceDir.absolutePath(); }
