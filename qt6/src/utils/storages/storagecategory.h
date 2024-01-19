// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_STORAGECATEGORY_H
#define ANTIQUACRM_UTILS_STORAGECATEGORY_H

#include <AntiquaWidgets>
#include <QCompleter>
#include <QMap>
#include <QWidget>

class StorageCategory final : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AntiquaCRM::AComboBox *m_box;
  AntiquaCRM::ALineEdit *m_find;
  QCompleter *m_completer;

private Q_SLOTS:
  void onKeyEnterPressed();

public Q_SLOTS:
  void initData() override;
  void setValue(const QVariant &) override;
  void setFocus() override;
  void reset() override;
  void search(const QString &str, Qt::ItemDataRole role = Qt::DisplayRole);

public:
  explicit StorageCategory(QWidget *parent = nullptr);
  void setRestrictions(const QSqlField &) override;
  void setInputToolTip(const QString &) override;
  void setBuddyLabel(const QString &) override;
  bool isValid() override;
  const QMetaType getType() const override;
  const QVariant getValue() override;
  const QString popUpHints() override;
  const QString statusHints() override;
  void addItems(const QMap<QString, QString> &map);
  const QPair<QString, QString> getItem(qint64 cid = 0);
  int itemCount();
};

#endif // ANTIQUACRM_UTILS_STORAGECATEGORY_H
