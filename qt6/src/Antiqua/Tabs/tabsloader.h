// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_TABS_LOADER_H
#define ANTIQUACRM_TABS_LOADER_H

#include <AGlobal>
#include <QDir>
#include <QList>
#include <QObject>
#include <QPluginLoader>

namespace AntiquaCRM {

class TabsInterface;

/**
 * @class TabsLoader
 * @brief Pluginloader class for this interface
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsLoader final : public QPluginLoader {
  Q_OBJECT

private:
  /**
   * @brief Antiquacrm plugin target
   * @ref AntiquaCRM::ASettings::getPluginDir("tabs")
   */
  QDir p_dir;

  /**
   * @brief Antiquacrm plugin search filter
   * @ref AntiquaCRM::ASettings::pluginSearchFilter()
   */
  QStringList p_filter;

public:
  /**
   * @param parent - parent object
   */
  explicit TabsLoader(QObject *parent = nullptr);

  /**
   * @brief get full interfaces list from plugin target
   *
   * Creates a plugin list with the specified plugin base path.
   * The absolute file paths are added to the list!
   */
  const QStringList getInterfaceList();

  /**
   * @brief find interface by name part
   *
   * Reads the full list from getInterfaceList() and looks for the name part.
   * Returns the Plugname. If Named plugin not found, a null String.
   *
   * @sa @ref getInterfaceList
   * @param name - part name
   */
  const QString getInterface(const QString &name);

  /**
   * @brief set interfaces with plugin name
   *
   * This function is an overload of QPluginLoader setFileName!
   * It calls getInterface() and performs an existence check before calling
   * setFilename.
   *
   * @sa @ref getInterface, QPluginLoader::setFileName
   * @param name - plugin name
   */
  bool setInterfaceName(const QString &name);

  /**
   * @brief load all interface into container
   *
   * Load and initial all Interfaces in this category
   *
   * @sa @ref getInterface, @ref AntiquaCRM::TabsInterface
   * @param parent - parent object
   */
  const QList<AntiquaCRM::TabsInterface *> interfaces(QObject *parent);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_LOADER_H
