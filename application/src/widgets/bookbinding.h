// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_BOOKBINDING_H
#define ANTIQUACRM_BOOKBINDING_H

#include <QWidget>
#include <AntiquaInputEdit>

/**
 * @brief The BookBinding class
 * @ingroup widgets
 */
class BookBinding : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit BookBinding(QWidget *parent = nullptr);
  void loadDataset() override;
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_BOOKBINDING_H
