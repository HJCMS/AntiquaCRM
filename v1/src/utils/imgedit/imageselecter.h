// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_IMAGESELECTER_H
#define ANTIQUA_IMAGESELECTER_H

#include <QDir>
#include <QFileInfo>
#include <QLineEdit>
#include <QList>
#include <QObject>
#include <QPushButton>
#include <QUrl>
#include <QWidget>

#include "imagelistview.h"
#include "sourceinfo.h"

class ImageSelecter : public QWidget {
  Q_OBJECT

private:
  QDir p_dir;
  QString p_selection;
  ImageListView *m_listView;
  QLineEdit *m_dirPathEdit;
  QPushButton *m_fileDialogBtn;

private Q_SLOTS:
  void selectImageTarget();

Q_SIGNALS:
  void sendTargetChanged(const QDir &dir);
  void sendSelection(const SourceInfo &image);

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
  const QString getSelection();
};

#endif // ANTIQUA_IMAGESELECTER_H
