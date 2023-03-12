// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_YEARSELECT_H
#define ANTIQUACRM_YEARSELECT_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QDate>
#include <QPushButton>

/**
 * @brief The YearSelect class
 * @ingroup widgets
 */
class YearSelect final : public InputEdit {
  Q_OBJECT

private:
  int factor = 2;
  QDate p_curDate;
  AntiquaComboBox *m_box;
  QPushButton *btn_open;
  void loadDataset() override;

private Q_SLOTS:
  void dataChanged(int);

Q_SIGNALS:
  void sendYearOpen();

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit YearSelect(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_YEARSELECT_H
