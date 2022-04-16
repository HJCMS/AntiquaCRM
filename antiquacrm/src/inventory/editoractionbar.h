// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EDITORACTIONBAR_H
#define EDITORACTIONBAR_H

#include <QtCore/QObject>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

class EditorActionBar : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QPushButton *m_restoreBtn;
  QPushButton *m_saveBtn;
  QPushButton *m_readyBtn;

Q_SIGNALS:
  void s_restoreClicked();
  void s_saveClicked();
  void s_finishClicked();

public Q_SLOTS:
  void setRestoreable(bool);

public:
  explicit EditorActionBar(QWidget *parent = nullptr);
  bool isRestoreable();
};

#endif // EDITORACTIONBAR_H
