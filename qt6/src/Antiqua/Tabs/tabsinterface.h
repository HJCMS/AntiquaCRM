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
#include <QJsonObject>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

class TabsIndex;
class PluginConfigWidget;

/**
 * @class TabsInterface
 * @brief Primary Interface class for Tabs.
 * @note Every tab interface plugin must have a menu item!
 *
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
   *
   * This is called in Menu entries
   */
  virtual const QString displayName() const = 0;

  /**
   * @brief Interface name
   *
   * Identifier name in ASCII, lowercase and no spaces.
   */
  virtual const QString interfaceName() const = 0;

  /**
   * @brief Which SQL Tablename in current Database
   *
   * SQL Table name, always used in @ref AntiquaCRM::ASqlQueryModel
   */
  virtual const QString sqlTableName() const = 0;

  /**
   * @brief Table column prefix without underscore
   *
   * All AntiquaCRM SQL Table Field Names must have a prefix!
   * This make it easier to use SQL JOIN Statements without Aliases.
   *
   * <b>Current Value Restrictions for this:</b>
   *  @li To Prevent duplicates in the SQL Database system, this Field prefix
   * must unique.
   *  @li This field prefix can only contain alphanumeric characters.
   *
   * @note An underscore is appended automatically!
   */
  virtual const QString sqlFieldPrefix() const = 0;

  /**
   * @brief Configuration Widget ...
   * @sa @ref AntiquaCRM::PluginConfigWidget
   * @param parent - parent object
   *
   * Widget for AntiquaCRM Configuration system.
   */
  virtual AntiquaCRM::PluginConfigWidget *configWidget(QWidget *parent) const = 0;

  /**
   * @brief if true - load this interface on application start into main
   */
  virtual bool addIndexOnInit() const = 0;

  /**
   * @brief Required Menu entry parameters.
   *
   * This important function defines the entry for application menu views.<br/>
   * The Json object schema is restricted and is described in the code block.
   *
   * @code
   *  QJsonObject {
   *    title:"<QString>", // Menue title
   *    id:"<QString>",    // AntiquaCRM::TabsIndex::objectName()
   *    icon:"<QString>"   // Icon name without extension
   *  }
   * @endcode
   */
  virtual const QJsonObject menuEntry() const = 0;

  /**
   * @brief Initial AntiquaCRM::TabsIndex widget.
   * @sa @ref AntiquaCRM::TabsIndex
   * @param parent - parent object
   *
   * Used in AntiquaCRM MainWindow @ref AntiquaCRM::TabsWidget::registerTab
   */
  virtual AntiquaCRM::TabsIndex *indexWidget(QWidget *parent) const = 0;

  /**
   * @brief Initial this interface
   * @param parent - parent object
   * @return boolean - true = create all additional function
   *
   * Reserved for interface initialization optimizations
   */
  virtual bool createInterface(QObject *parent) = 0;
};

} // namespace AntiquaCRM

Q_DECLARE_INTERFACE(AntiquaCRM::TabsInterface, ANTIQUACRM_TABS_INTERFACE);

#endif // ANTIQUACRM_TABS_INTERFACE_H
