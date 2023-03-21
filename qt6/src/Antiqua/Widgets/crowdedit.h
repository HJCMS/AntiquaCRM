// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_CROWDEDIT_H
#define ANTIQUACRM_WIDGETS_CROWDEDIT_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaWidgets
 * @class CrowdEdit
 */
class ANTIQUACRM_LIBRARY CrowdEdit final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  AntiquaCRM::ASpinBox *m_edit;

private Q_SLOTS:
  /**
   * @brief crowd has been changed
   */
  void valueChanged(int);

protected:
  /**
   * @brief create basic dataset construction
   * @note Field name or Object name is not set in this function.
   */
  virtual void initData() override;

public Q_SLOTS:
  /**
   * @brief set current article count
   */
  virtual void setValue(const QVariant &) override;

  /**
   * @brief set input focus
   */
  virtual void setFocus() override;

  /**
   * @brief restore to minium value
   */
  virtual void reset() override;

public:
  /**
   * @brief Article count editor
   * @param parent - parent widget
   */
  explicit CrowdEdit(QWidget *parent = nullptr);

  /**
   * @brief Configure data restriction from QSqlQuery fieldset.
   */
  virtual void setRestrictions(const QSqlField &) override;

  /**
   * @brief Configure tooltip for this input edit widget
   */
  virtual void setInputToolTip(const QString &) override;

  /**
   * @brief Add a buddy label to this input widget.
   */
  virtual void setBuddyLabel(const QString &) override;

  /**
   * @brief is it a valid article count?
   */
  virtual bool isValid() override;

  /**
   * @brief get current article count
   */
  virtual const QVariant getValue() override;

  /**
   * @brief Detailed warn message for QMessageBox
   */
  virtual const QString popUpHints() override;

  /**
   * @brief Simple warn message for QStatusBar
   */
  virtual const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_CROWDEDIT_H
