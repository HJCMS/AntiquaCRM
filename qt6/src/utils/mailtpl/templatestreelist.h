// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_MAILTPLSTREELIST_H
#define ANTIQUACRM_UTILS_MAILTPLSTREELIST_H

#include <QJsonObject>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

class TemplatesTreeList final : public QTreeWidget {
  Q_OBJECT

private:
  QTreeWidgetItem *getParent(const QString &id);
  void createCategories();
  void insertTemplate(const QJsonObject &);

private Q_SLOTS:
  void templateClicked(QTreeWidgetItem *, int);

Q_SIGNALS:
  void sendTemplate(const QJsonObject &);

public:
  explicit TemplatesTreeList(QWidget *parent = nullptr);
  bool cleanup();
  void addTemplates(const QJsonObject &);
};

#endif // ANTIQUACRM_UTILS_MAILTPLSTREELIST_H
