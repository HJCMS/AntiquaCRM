// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef IMGEDIT_H
#define IMGEDIT_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

class ImgEdit : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_imageInfo;
  QSlider *m_gammaSlider;
  QListWidget *m_listFiles;

public Q_SLOTS:
  void addFileList(const QStringList &l);

public:
  explicit ImgEdit(QWidget *parent = nullptr);
};

#endif // IMGEDIT_H
