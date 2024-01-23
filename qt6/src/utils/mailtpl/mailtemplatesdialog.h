// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_MAILTPLDIALOG_H
#define ANTIQUACRM_UTILS_MAILTPLDIALOG_H

#include <AntiquaWidgets>
#include <QCheckBox>
#include <QLineEdit>
#include <QJsonObject>

class MailTemplatesTree;
class TemplateEditor;

class MailTemplatesDialog final : public AntiquaCRM::ADialog {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql = nullptr;
  QFrame *m_frame;
  QCheckBox *m_tb_activ;
  QCheckBox *m_tb_attachment;
  QLineEdit *m_tb_caller;
  QLineEdit *m_tb_category;
  QLineEdit *m_tb_subject;
  QLineEdit *m_tb_tab;
  QLineEdit *m_tb_title;
  AntiquaCRM::Splitter *m_splitter;
  TemplateEditor *m_edit;
  MailTemplatesTree *m_treeMenu;
  const QString boolSet(const QString &field, bool b = false) const;

private Q_SLOTS:
  void openTemplate(const QJsonObject &);
  void save();

public:
  explicit MailTemplatesDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif // ANTIQUACRM_UTILS_MAILTPLDIALOG_H
