// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MESSAGETEMPLATES_UTILS_H
#define MESSAGETEMPLATES_UTILS_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QObject>
#include <QWidget>
#include <QStatusBar>
#include <QSplitter>

#include <SqlCore>

class MessageSelecter;
class MessageEditor;
class MessageKeywordList;

class MessageTemplates : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  MessageSelecter *m_toolBar;
  QSplitter *m_splitter;
  MessageEditor *m_editor;
  MessageKeywordList *m_keysList;
  QDialogButtonBox *m_btnBox;
  QStatusBar *m_statusBar;

  const QString querySection(const QString &) const;

//private Q_SLOTS:

//Q_SIGNALS:

public:
  explicit MessageTemplates(QWidget *parent = nullptr);
  int exec() override;
};

#endif // MESSAGETEMPLATES_UTILS_H
