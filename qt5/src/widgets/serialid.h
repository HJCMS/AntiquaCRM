// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_SERIALID_H
#define ANTIQUACRM_SERIALID_H

#include <AntiquaInputEdit>
#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QVariant>

/**
 * @brief The SerialID class
 * @ingroup widgets
 */
class SerialID final : public InputEdit {
  Q_OBJECT

private:
  QLabel *m_serialLabel;

private Q_SLOTS:
  void loadDataset() override;

Q_SIGNALS:
  void sendSerialChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &) override;
  Q_INVOKABLE void reset() override;
  void setFocus() override;

public:
  explicit SerialID(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QVariant value() override;
  bool isValid() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_SERIALID_H
