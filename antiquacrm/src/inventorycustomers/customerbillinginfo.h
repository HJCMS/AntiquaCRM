// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CUSTOMERBILLINGINFO_H
#define CUSTOMERBILLINGINFO_H

#include <QtCore/QObject>
#include <QtWidgets/QWidget>

class CustomerBillingInfo : public QWidget
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:

Q_SIGNALS:

public Q_SLOTS:

public:
  explicit CustomerBillingInfo(QWidget *parent = nullptr);

};

#endif // CUSTOMERBILLINGINFO_H

