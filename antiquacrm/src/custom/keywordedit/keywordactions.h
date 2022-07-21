// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef KEYWORD_ACTIONS_H
#define KEYWORD_ACTIONS_H

#include <QFrame>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class KeywordActions : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QPushButton *m_btnReload;
  QPushButton *m_btnQuit;

Q_SIGNALS:
  void sendReload();
  void sendQuit();

public:
  explicit KeywordActions(QWidget *parent = nullptr);
};

#endif // KEYWORD_ACTIONS_H
