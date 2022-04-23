// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef URLEDIT_H
#define URLEDIT_H

#include <QtCore/QObject>
#include <QtWidgets/QWidget>

class UrlEdit : public QWidget
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:

Q_SIGNALS:

public Q_SLOTS:

public:
  explicit UrlEdit(QWidget *parent = nullptr);

};

#endif // URLEDIT_H

