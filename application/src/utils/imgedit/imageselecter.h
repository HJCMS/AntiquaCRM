// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_IMAGESELECTER_H
#define ANTIQUA_IMAGESELECTER_H

#include <QAction>
#include <QDir>
#include <QFileInfo>
#include <QLineEdit>
#include <QList>
#include <QObject>
#include <QPushButton>
#include <QToolBar>
#include <QUrl>
#include <QWidget>

#include "imagelistview.h"
#include "sourceinfo.h"

class ImageSelecter : public QWidget {
  Q_OBJECT

private:
  QDir p_dir;
  QLineEdit *m_dirPathEdit;
  QPushButton *m_fileDialogBtn;
  ImageListView *m_listView;
  QToolBar *m_toolbar;
  QAction *ac_set_archive;
  QAction *ac_set_camera;
  QAction *ac_set_image;

private Q_SLOTS:
  void setDefaultTarget();
  void setImportTarget();
  void selectImageTarget();
  void selectSingleImage();

Q_SIGNALS:
  void sendTargetChanged(const QDir &dir);
  void sendSelection(const SourceInfo &image);
  void sendImportImage(const QString &import);
  void sendStatusMessage(const QString &);

public Q_SLOTS:
  void setDirectory(const QString &dirPath);
  void setSelection(const SourceInfo &src);

public:
  explicit ImageSelecter(QWidget *parent = nullptr);
#ifdef Q_OS_WIN
  /**
   * @brief Not needed for Linux Systems
   */
  static const QList<QUrl> sideBarUrls();
#endif
  const QDir directory();
};

#endif // ANTIQUA_IMAGESELECTER_H
