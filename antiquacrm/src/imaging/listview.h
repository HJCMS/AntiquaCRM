// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QtCore/QObject>
#include <QtCore/QModelIndex>
#include <QtCore/QFileInfo>
#include <QtWidgets/QListView>

class FileViewModel;

class ListView : public QListView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  FileViewModel *m_model;
  bool checkJpeg(const QFileInfo &);

private Q_SLOTS:
  void checkSingleClicked(const QModelIndex &);
  void checkDoubleClicked(const QModelIndex &);

Q_SIGNALS:
  void previewImage(const QString &);
  void selectionChanged(const QString &);

public Q_SLOTS:
  void goTo(const QString &);
  void goUpward();

public:
  explicit ListView(QWidget *parent = nullptr);
};

#endif // LISTVIEW_H
