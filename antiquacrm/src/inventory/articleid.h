// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ARTICLEID_H
#define ARTICLEID_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

class ArticleID : public QLineEdit {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();

public:
  explicit ArticleID(QWidget *parent = nullptr);
  Q_INVOKABLE bool hasModified();
  const QVariant value();
};

#endif // ARTICLEID_H
