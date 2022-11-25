// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INFOPAGE_H
#define INFOPAGE_H

#include <QLabel>
#include <QObject>
#include <QWidget>

class InfoPage : public QWidget {
  Q_OBJECT

private:
  QLabel *m_text;

public:
  explicit InfoPage(QWidget *parent = nullptr);
  void setContent(const QString &key);
};

#endif // INFOPAGE_H
