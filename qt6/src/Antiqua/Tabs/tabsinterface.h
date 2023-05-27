// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_INTERFACE_H
#define ANTIQUACRM_TABS_INTERFACE_H

#include <AGlobal>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

class TabsIndex;
class TabsConfigWidget;

/**
 * @class TabsInterface
 * @brief Primary Interface class for Tabs.
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsInterface : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Interface", ANTIQUACRM_TABS_INTERFACE)
  Q_CLASSINFO("Description", "AntiquaCRM TabWidgets Interface")
  Q_CLASSINFO("Version", ANTIQUACRM_VERSION)
  Q_CLASSINFO("Url", ANTIQUACRM_HOMEPAGE)

public:
  /**
   * @param parent - parent object
   */
  explicit TabsInterface(QObject *parent = nullptr);

  /**
   * @brief Interface Title and Display name
   */
  virtual const QString displayName() const = 0;

  /**
   * @brief Interface name
   * Identifier name in ASCII, lowercase and no spaces.
   */
  virtual const QString interfaceName() const = 0;

  /**
   * @brief Used SQL Table name in Database
   */
  virtual const QString sqlTableName() const = 0;

  /**
   * @brief Table column prefix without underscore
   */
  virtual const QString sqlFieldPrefix() const = 0;

  /**
   * @brief Configuration Widget ...
   *
   * for AntiquaCRM application configuration Dialog
   *
   * @sa @ref AntiquaCRM::TabsConfigWidget
   * @param parent - parent object
   */
  virtual AntiquaCRM::TabsConfigWidget *configWidget(QWidget *parent) const = 0;

  /**
   * @brief if true - load this interface on application start into main
   */
  virtual bool addIndexOnInit() const = 0;

  /**
   * @brief load the TabsIndex widget
   *
   * @sa @ref AntiquaCRM::TabsIndex
   * @param parent - parent object
   */
  virtual AntiquaCRM::TabsIndex *indexWidget(QWidget *parent) const = 0;

  /**
   * @brief Initial this interface
   *
   * reserved for configuration and object initialisation
   *
   * @param parent - parent object
   */
  virtual bool createInterface(QObject *parent) = 0;
};

} // namespace AntiquaCRM

Q_DECLARE_INTERFACE(AntiquaCRM::TabsInterface, ANTIQUACRM_TABS_INTERFACE)

#endif // ANTIQUACRM_TABS_INTERFACE_H
