// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef FILEVIEWMODEL_H
#define FILEVIEWMODEL_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtWidgets/QFileIconProvider>
#include <QtWidgets/QFileSystemModel>

class FileViewModel : public QFileSystemModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QDir p_curDir;

private Q_SLOTS:
  void currentTarget(const QString &path);

public:
  explicit FileViewModel(QObject *parent = nullptr);
  const QDir last();
};

#endif // FILEVIEWMODEL_H
