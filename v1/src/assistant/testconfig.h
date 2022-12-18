// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TESTCONFIG_H
#define TESTCONFIG_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

class TestConfig : public QWidget {
  Q_OBJECT

private:
  QTextEdit *m_info;

Q_SIGNALS:
  void startTest();

public Q_SLOTS:
  void setBodyText(const QString &);

public:
  explicit TestConfig(QWidget *parent = nullptr);
};

#endif // TESTCONFIG_H
