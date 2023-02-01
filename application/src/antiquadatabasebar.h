// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_DATABASEBAR_H
#define ANTIQUACRM_DATABASEBAR_H

#include <QObject>
#include <QToolBar>
#include <QWidget>

class AntiquaDatabaseBar final : public QToolBar {
  Q_OBJECT

private:
  QAction *ac_status;

private Q_SLOTS:
  void databaseInfoDialog();

public Q_SLOTS:
  void setStatus(bool);

public:
  explicit AntiquaDatabaseBar(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_DATABASEBAR_H
