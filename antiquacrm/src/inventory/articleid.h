// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ARTICLEID_H
#define ARTICLEID_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

class ArticleID : public QLineEdit
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

public:
  explicit ArticleID(QWidget *parent = nullptr);
  void setValue(const QVariant &);
  const QVariant value();
};

#endif // ARTICLEID_H

