// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PROVIDER_INTERFACE_H
#define ANTIQUACRM_PROVIDER_INTERFACE_H

#include <AGlobal>
#include <QJsonObject>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

class PluginConfigWidget;

/**
 * @class ProviderInterface
 * @brief Primary Interface class for Provider.
 * @note Every provider interface plugin must have a config entry!
 *
 * @ingroup ProviderInterface
 */
class ANTIQUACRM_LIBRARY ProviderInterface : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Interface", ANTIQUACRM_PROVIDER_INTERFACE)
  Q_CLASSINFO("Description", "AntiquaCRM Provider Interface")
  Q_CLASSINFO("Version", ANTIQUACRM_VERSION)
  Q_CLASSINFO("Url", ANTIQUACRM_HOMEPAGE)

public:
  /**
   * @param parent - parent object
   */
  explicit ProviderInterface(QObject *parent = nullptr);

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
   * @brief Configuration Widget ...
   * @sa @ref AntiquaCRM::TabsConfigWidget
   * @param parent - parent object
   *
   * Widget for AntiquaCRM Configuration system.
   */
  virtual AntiquaCRM::PluginConfigWidget *configWidget(QWidget *parent) const = 0;

  /**
   * @brief Required Menu entry parameters.
   *
   * This important function defines the entry for application menu views.<br/>
   * The Json object schema is restricted and is described in the code block.
   *
   * @code
   *  QJsonObject {
   *    title:"<QString>", // Menue title
   *    id:"<QString>",    // AntiquaCRM::ProviderIndex::objectName()
   *    icon:"<QString>"   // Icon name without extension
   *  }
   * @endcode
   */
  virtual const QJsonObject menuEntry() const = 0;

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

Q_DECLARE_INTERFACE(AntiquaCRM::ProviderInterface,
                    ANTIQUACRM_PROVIDER_INTERFACE);

#endif // ANTIQUACRM_PROVIDER_INTERFACE_H
