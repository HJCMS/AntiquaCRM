// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ANTIQUAWINDOW_H
#define ANTIQUACRM_ANTIQUAWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

class AntiquaWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit AntiquaWindow(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_ANTIQUAWINDOW_H
