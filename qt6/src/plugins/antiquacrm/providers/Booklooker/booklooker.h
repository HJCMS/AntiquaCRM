// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKLOOKER_H
#define ANTIQUACRM_PLUGIN_BOOKLOOKER_H

#include "booklookerconfig.h"
#include <AntiquaProviders>
#include <QJsonObject>
#include <QObject>
#include <QWidget>

/**
 * @class BookLooker
 * @brief BookLooker Provider Interface
 *
 * @ingroup BookLooker
 */
class BOOKLOOKER_PLUGIN BookLooker final : public AntiquaCRM::ProviderInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_PROVIDER_INTERFACE FILE "booklooker.json")
  Q_INTERFACES(AntiquaCRM::ProviderInterface)

public:
  /**
   * @param parent - parent object
   */
  explicit BookLooker(QObject* parent = nullptr);

  /**
   * @brief Interface Title and Display name
   *
   * This is called in Menu entries
   */
  virtual const QString displayName() const override;

  /**
   * @brief Interface name
   *
   * Identifier name in ASCII, lowercase and no spaces.
   */
  virtual const QString interfaceName() const override;

  /**
   * @brief Configuration Widget ...
   * @sa @ref AntiquaCRM::TabsConfigWidget
   * @param parent - parent object
   *
   * Widget for AntiquaCRM Configuration system.
   */
  virtual AntiquaCRM::PluginConfigWidget* configWidget(QWidget* parent) const override;

  virtual bool operationWidget(QWidget* parent, const QJsonObject& order) const override;

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
  virtual const QJsonObject menuEntry() const override;

  /**
   * @brief Initial this interface
   * @param parent - parent object
   * @return boolean - true = create all additional function
   *
   * Reserved for interface initialization optimizations
   */
  virtual bool createInterface(QObject* parent) override;
};

#endif // ANTIQUACRM_PLUGIN_BOOKLOOKER_H
