// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef KEYWORD_EDIT_H
#define KEYWORD_EDIT_H

#include <QDialog>
#include <QJsonObject>
#include <QObject>
#include <QStatusBar>
#include <QLineEdit>
#include <QToolBar>
#include <QWidget>

#include <SqlCore>

class KeywordTable;
class KeywordActions;
class KeywordEditor;

class KeywordEdit : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  QToolBar *m_searchBar;
  QLineEdit *m_searchLine;
  KeywordTable *m_table;
  KeywordEditor *m_editor;
  KeywordActions *m_actionsBar;

  bool initBooklooker();

private Q_SLOTS:
  void searchKeyword();
  void queryKeywordEntry(int id);
  void updateKeywordEntry(const QJsonObject &);
  void insertKeywordEntry(const QJsonObject &);

public:
  explicit KeywordEdit(QWidget *parent = nullptr);
  int exec() override;
};

#endif // KEYWORD_EDIT_H
