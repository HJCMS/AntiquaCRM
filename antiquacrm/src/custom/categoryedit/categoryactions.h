// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYEDIT_ACTIONS_H
#define CATEGORYEDIT_ACTIONS_H

#include <QFrame>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class CategoryActions : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QPushButton *m_btnSave;
  QPushButton *m_btnToggle;
  QPushButton *m_btnVisible;
  QPushButton *m_btnQuit;

Q_SIGNALS:
  void sendQuitDialog();
  void sendSaveDialog();
  void sendViewTree();
  void sendVisible();

public:
  explicit CategoryActions(QWidget *parent = nullptr);
};

#endif // CATEGORYEDIT_ACTIONS_H
