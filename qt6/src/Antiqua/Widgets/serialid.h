// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SERIALID_H
#define ANTIQUACRM_WIDGETS_SERIALID_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class SerialId
 * @brief Display Table Id's e.g. ArticleId etc.
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY SerialId final : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  ASpinBox *m_edit;

private Q_SLOTS:
  void valueChanged();

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
  explicit SerialId(QWidget *parent = nullptr);

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QMetaType getType() const override;

  /**
   * @brief Current SerialId
   * @note If no valid Serial exists it returns NULL and not Digits!
   * @return 1-9999999 or NULL
   */
  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SERIALID_H
