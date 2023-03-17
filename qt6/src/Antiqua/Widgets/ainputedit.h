// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_WIDGETS_INPUTEDIT_H
#define ANTIQUA_WIDGETS_INPUTEDIT_H

#include <AGlobal>
#include <ASettings>
#include <QObject>
#include <QSqlField>
#include <QVariant>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaWidgets
 * @class AInputEdit
 * @brief Abstract AInputEdit class
 */
class ANTIQUACRM_LIBRARY AInputEdit {

private:
  /**
   * @brief parent object handle for AntiquaCRM::ASettings
   * @see behaviorSettings
   */
  QObject *parent;

  /**
   * @brief default required input status
   */
  bool required_status = false;

protected:
  /**
   * @brief Configuration caller
   */
  AntiquaCRM::ASettings *m_cfg;

  /**
   * @brief Widget behavior settings
   * @param key - call with key from settings group "window_behavior".
   * @param standard - deault value for caller
   */
  bool behaviorSettings(const QString &key, bool standard = false);

  /**
   * @brief is this input required by sql database?
   */
  void setRequired(bool);

public Q_SLOTS:
  /**
   * @brief set value by multiple data types
   */
  virtual void setValue(const QVariant &) = 0;

  /**
   * @brief find input restriction with QSqlField info
   */
  virtual void setRestrictions(const QSqlField &) = 0;

public:
  /**
   * @brief AInputEdit
   * @param parent - important we need a handle for AntiquaCRM::ASettings
   */
  explicit AInputEdit(QObject *parent);

  /**
   * @brief is this input required by sql database?
   */
  bool isRequired();

  /**
   * @brief get data value from ...
   */
  virtual const QVariant getValue() = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUA_WIDGETS_INPUTEDIT_H
