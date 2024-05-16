// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_MAILTPLTREE_H
#define ANTIQUACRM_UTILS_MAILTPLTREE_H

#include <AntiquaWidgets>
#include <QTabWidget>

class TemplatesTreeList;
class MacrosTreeList;

class MailTemplatesTree final : public QTabWidget {
  Q_OBJECT

private:
  TemplatesTreeList *m_templates;
  MacrosTreeList *m_macros;
  bool createTemplatesTree(AntiquaCRM::ASqlCore *);
  bool createMacrosTree(AntiquaCRM::ASqlCore *);

Q_SIGNALS:
  void sendTemplate(const QJsonObject &);

public:
  explicit MailTemplatesTree(QWidget *parent = nullptr);
  bool init(AntiquaCRM::ASqlCore *);
};

#endif // ANTIQUACRM_UTILS_MAILTPLTREE_H
