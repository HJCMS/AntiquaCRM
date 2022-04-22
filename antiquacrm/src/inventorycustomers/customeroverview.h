// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CUSTOMEROVERVIEW_H
#define CUSTOMEROVERVIEW_H

#include <QtCore/QObject>
#include <QtWidgets/QWidget>

class CustomerOverview : public QWidget
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:

Q_SIGNALS:

public Q_SLOTS:

public:
  explicit CustomerOverview(QWidget *parent = nullptr);

};

#endif // CUSTOMEROVERVIEW_H

