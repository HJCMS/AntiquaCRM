// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H

#include <QtCore/QObject>
#include <QtGui/QDoubleValidator>
#include <QtGui/QRegExpValidator>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

class SearchLineEdit : public QLineEdit {
  Q_OBJECT

private:
  QRegExpValidator *m_textValidator;
  QDoubleValidator *m_idValidator;
  QCompleter *m_completer;

private Q_SLOTS:
  void prepareQuery(const QString &);

Q_SIGNALS:
  void inputChanged(const QString &);

public Q_SLOTS:
  void updatePlaceHolder(int);

public:
  explicit SearchLineEdit(QWidget *parent = nullptr);
};

#endif // SEARCHLINEEDIT_H
