// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_AMENU_H
#define ANTIQUACRM_AMENU_H

#include <AGlobal>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QSignalMapper>

namespace AntiquaCRM {

/**
 * @class AMenu
 * @brief Menu main class for Antiqua Tab action menu
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY AMenu : public QMenu {
  Q_OBJECT

protected:
  /**
   * @brief Signal Mapper
   * @note The mapper is already initiated in constructor!
   *
   * Signal "mappedString" is registered by default and will forwarded to signal
   * "sendOpenTab".
   */
  QSignalMapper *m_mapper;

  /**
   * @brief initial your action menue
   */
  virtual void addActions() = 0;

Q_SIGNALS:
  /**
   * @param Sending Action ID to open
   */
  void sendOpenTab(const QString &);

public:
  /**
   * @param parent - parent object
   */
  explicit AMenu(QMenuBar *parent);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_AMENU_H
