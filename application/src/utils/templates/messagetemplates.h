// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TEMPLATES_MESSAGETEMPLATES_H
#define TEMPLATES_MESSAGETEMPLATES_H

#include <AntiquaCRM>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QObject>
#include <QPushButton>
#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QWidget>

class TemplatesTree;
class TemplatesHeader;
class MessageEditor;
class MessageKeywordList;

class MessageTemplates final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  TemplatesHeader *m_toolBar;
  QSplitter *m_splitter;
  QTabWidget *m_tabWidget;
  TemplatesTree *m_tplTree;
  MessageEditor *m_editor;
  MessageKeywordList *m_keysList;
  QDialogButtonBox *m_btnBox;
  QPushButton *btn_save;
  QStatusBar *m_statusBar;

  AntiquaCRM::ASqlDataQuery *m_tplData;

  bool createMacrosTree();

  bool createTemplateTree();

private Q_SLOTS:
  void saveCurrentTemplate();
  void setEditTemplate(const QJsonObject &);

public:
  explicit MessageTemplates(QWidget *parent = nullptr);
  int exec() override;
};

#endif // TEMPLATES_MESSAGETEMPLATES_H
