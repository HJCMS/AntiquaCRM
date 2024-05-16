// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SELECTMEDIATYPE_H
#define ANTIQUACRM_WIDGETS_SELECTMEDIATYPE_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class SelectMediaType
 * @brief The „SelectMediaType“ input edit class.
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY SelectMediaType final
    : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AComboBox *m_edit;

private Q_SLOTS:
  void valueChanged(int);

protected:
  void initData() override;

public Q_SLOTS:
  void setValue(const QVariant &) override;
  void setFocus() override;
  void reset() override;

public:
  /**
   * @param parent - parent widget
   */
  explicit SelectMediaType(QWidget *parent = nullptr);

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

#endif // ANTIQUACRM_WIDGETS_SELECTMEDIATYPE_H
