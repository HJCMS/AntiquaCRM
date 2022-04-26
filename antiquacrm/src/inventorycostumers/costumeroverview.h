// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COSTUMEROVERVIEW_H
#define COSTUMEROVERVIEW_H

#include <QtCore/QObject>
#include <QtWidgets/QWidget>

class CostumerOverview : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit CostumerOverview(QWidget *parent = nullptr);
};

#endif // COSTUMEROVERVIEW_H
