// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_GENDERBOX_H
#define ANTIQUACRM_GENDERBOX_H

#include <AGlobal>
#include <AntiquaInputEdit>

/**
 * @brief The GenderBox class
 * @ingroup widgets
 */
class GenderBox final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  void loadDataset() override{};

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit GenderBox(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QHash<AntiquaCRM::Gender, QString> genderData();
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_GENDERBOX_H
