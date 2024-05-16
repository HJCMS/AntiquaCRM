// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SELECTDELIVERSERVICE_H
#define ANTIQUACRM_WIDGETS_SELECTDELIVERSERVICE_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY SelectDeliverService
    : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AComboBox *m_edit;

private Q_SLOTS:
  void valueChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &) override;
  void setFocus() override;
  void reset() override;

Q_SIGNALS:
  void sendSelectedService(int);

public:
  /**
   * @param parent - parent widget
   */
  explicit SelectDeliverService(QWidget *parent = nullptr);
  void initData() override;
  void setRestrictions(const QSqlField &) override;
  void setInputToolTip(const QString &) override;
  void setBuddyLabel(const QString &) override;
  bool isValid() override;
  const QMetaType getType() const override;
  const QVariant getValue() override;
  const QString popUpHints() override;
  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SELECTDELIVERSERVICE_H
