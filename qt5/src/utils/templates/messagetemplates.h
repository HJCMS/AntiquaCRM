// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

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

/**
 * @ingroup _templates
 * @brief Text Templates for eMail and Printing
 *
 * @class MessageTemplates
 * @brief Edit Message Templates dialog
 */
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
