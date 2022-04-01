// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SEARCHTOOLBAR_H
#define SEARCHTOOLBAR_H

#include <QtCore/QObject>
#include <QtGui/QRegExpValidator>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

class ComboFieldBox;
class SearchLineEdit;
struct SearchStatement;

class SearchBookBar : public QToolBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  int minlength;
  SearchLineEdit *m_input;
  ComboFieldBox *m_comboBox;

private Q_SLOTS:
  void lineEditChanged(const QString &);
  void startSearchClicked();

public:
  explicit SearchBookBar(QWidget *main);

Q_SIGNALS:
  void s_sqlQueryChanged(const SearchStatement &);
};

#endif // SEARCHTOOLBAR_H
