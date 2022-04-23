// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QWidget>

class FileDialog : public QFileDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QDir p_dir;
  void findSidebarUrls();
  bool checkFile(const QString &);

protected Q_SLOTS:
  virtual void accept();

public:
  explicit FileDialog(QWidget *parent = nullptr);
  void setImageFilter();
  void setCertsFilter();
  void setPdfFilter();
};

#endif // FILEDIALOG_H
