// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_EMAILEDIT_H
#define ANTIQUACRM_EMAILEDIT_H

#include <AntiquaInputEdit>
#include <QAction>
#include <QRegExpValidator>

/**
 * @brief The EMailEdit class
 * @ingroup widgets
 */
class EMailEdit final : public InputEdit {
  Q_OBJECT

private:
  AntiquaLineEdit *m_edit;
  QAction *ac_copy;
  QRegExpValidator *m_validator;
  void loadDataset() override{};
  bool validate(const QString &mail) const;

private Q_SLOTS:
  void pushStatusEvent(const QString &);
  void dataChanged(const QString &);
  void copyIntoClipboard();

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit EMailEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_EMAILEDIT_H
