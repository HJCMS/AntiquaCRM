// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_MAILTPLMACROSTREELIST_H
#define ANTIQUACRM_UTILS_MAILTPLMACROSTREELIST_H

#include <QJsonObject>
#include <QMouseEvent>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

class MacrosTreeList final : public QTreeWidget {
  Q_OBJECT

private:
  QString delimiter = QString("@");
  QTreeWidgetItem *getSection(const QString &section, const QString &type);
  bool insertMacro(QTreeWidgetItem *parent, const QJsonObject &jsObj);

protected:
  void mousePressEvent(QMouseEvent *event) override;

Q_SIGNALS:
  void sendMacro(const QString &);

public Q_SLOTS:
  void addMacro(const QString &section, const QJsonObject &jsObj);

public:
  explicit MacrosTreeList(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_UTILS_MAILTPLMACROSTREELIST_H
