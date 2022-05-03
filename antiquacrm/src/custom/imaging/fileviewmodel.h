// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef FILEVIEWMODEL_IMAGING_H
#define FILEVIEWMODEL_IMAGING_H

#include <QObject>
#include <QString>
#include <QFileIconProvider>
#include <QFileSystemModel>

class FileViewModel : public QFileSystemModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
     Aktuelle Verzeichnisebene
   */
  QDir p_curDir;

private Q_SLOTS:
  /**
     Setzt @ref p_curDir bei einem
     Verzeichniswechsel oder wenn der Wurzelpfad
     geändert wurde.
   */
  void currentTarget(const QString &path);

public:
  explicit FileViewModel(QObject *parent = nullptr);

  /**
    Übergibt das zuletzt gesetzte Verzeichnis zurück.
  */
  const QDir last();
};

#endif // FILEVIEWMODEL_IMAGING_H
