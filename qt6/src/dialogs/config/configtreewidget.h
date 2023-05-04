// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_TREEWIDGET_H
#define ANTIQUACRM_DIALOGS_TREEWIDGET_H

#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class ConfigTreeWidget final : public QTreeWidget {
  Q_OBJECT

private:
  QTreeWidgetItem *m_mainIndex;
  QTreeWidgetItem *m_tabsIndex;
  QTreeWidgetItem *m_providerIndex;

  void addChild(QTreeWidgetItem *main,
                int page,             // section
                const QString &title, // title
                const QIcon &icon);

private Q_SLOTS:
  void setItemSelected(QTreeWidgetItem *, int);

Q_SIGNALS:
  void sendPageIndex(int);

public Q_SLOTS:
  void addGeneral(int page, const QString &title, const QIcon &icon);
  void addTabPlugin(int page, const QString &title, const QIcon &icon);
  void addProviderPlugin(int page, const QString &title, const QIcon &icon);

public:
  explicit ConfigTreeWidget(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_DIALOGS_TREEWIDGET_H
