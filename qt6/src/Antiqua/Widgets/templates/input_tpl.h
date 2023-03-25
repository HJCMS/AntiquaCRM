// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: @YEAR@ Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_@CLASSMACRO@_H
#define ANTIQUACRM_WIDGETS_@CLASSMACRO@_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaWidgets
 * @class @CLASSNAME@
 */
class ANTIQUACRM_LIBRARY @CLASSNAME@ final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  // *m_edit;

private Q_SLOTS:
  // void valueChanged(??);

protected:
  virtual void initData() override;

public Q_SLOTS:
  virtual void setValue(const QVariant &) override;

  virtual void setFocus() override;

  virtual void reset() override;

public:
  /**
   * @brief __TODO__
   * @param parent - parent widget
   */
  explicit @CLASSNAME@(QWidget *parent = nullptr);

  virtual void setRestrictions(const QSqlField &) override;

  virtual void setInputToolTip(const QString &) override;

  virtual void setBuddyLabel(const QString &) override;

  virtual bool isValid() override;

  virtual const QVariant getValue() override;

  virtual const QString popUpHints() override;

  virtual const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_@CLASSMACRO@_H
