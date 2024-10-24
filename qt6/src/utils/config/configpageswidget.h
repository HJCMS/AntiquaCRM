// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGPAGESWIDGET_H
#define ANTIQUACRM_DIALOGS_CONFIGPAGESWIDGET_H

#include <AntiquaTabs>
#include <QEvent>
#include <QObject>
#include <QStackedWidget>

class ConfigPagesWidget final : public QStackedWidget {
  Q_OBJECT

protected Q_SLOTS:
  bool eventFilter(QObject *, QEvent *) override;
  void changeEvent(QEvent *) override;

Q_SIGNALS:
  void sendModified(bool);
  void sendEnabledStatus(bool);
  void sendPageTitle(const QString &);

public Q_SLOTS:
  void setPage(int);

public:
  explicit ConfigPagesWidget(QWidget *parent = nullptr);

  int insert(int index, AntiquaCRM::PluginConfigWidget *widget);

  const QList<AntiquaCRM::PluginConfigWidget *> pages();

  AntiquaCRM::PluginConfigWidget *page(int index);

  AntiquaCRM::PluginConfigWidget *page(QObject *);
};

#endif // ANTIQUACRM_DIALOGS_CONFIGPAGESWIDGET_H
