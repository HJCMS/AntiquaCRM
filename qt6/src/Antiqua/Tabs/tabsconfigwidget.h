// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_CONFIGWIDGET_H
#define ANTIQUACRM_TABS_CONFIGWIDGET_H

#include <AntiquaWidgets>
#include <QIcon>
#include <QMap>
#include <QMetaType>
#include <QObject>
#include <QScrollArea>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class TabsConfigWidget
 * @brief Abstract System Configuration Widget
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsConfigWidget : public QScrollArea {
  Q_OBJECT

private:
  const QString p_gid;

protected:
  /**
   * @brief Global settings instance
   * Always initialized in constructor!
   */
  AntiquaCRM::ASettings *config;

  /**
   * @brief PostgreSql Database Connection
   * @note By default not initialized!
   * @warning Do not initialize SQL Connections in Constructors!
   */
  AntiquaCRM::ASqlCore *pgsql = nullptr;

  /**
   * @brief get all AntiquaCRM::AbstractInput from Widget
   * @param parent
   */
  QList<AntiquaCRM::AbstractInput *> getInputList(QObject *parent);

Q_SIGNALS:
  void sendHasModified(bool);

public Q_SLOTS:
  /**
   * @brief Loading configuration for...
   */
  virtual void loadSectionConfig() = 0;

  /**
   * @brief Save current settings.
   */
  virtual void saveSectionConfig() = 0;

public:
  enum ConfigType {
    CONFIG_SYSTEM = 0,  /**< @brief Default */
    CONFIG_DATABASE = 1 /**< @brief stored in database */
  };

  /**
   * @param group  - Configuration group
   * @param id     - Configuration identifier
   * @param parent - Parent Widget
   */
  explicit TabsConfigWidget(const QString &group, // config group
                            const QString &id,    // config id
                            QWidget *parent = nullptr);

  ~TabsConfigWidget();

  /**
   * @brief configuration group path
   */
  const QString getGroup() const;

  /**
   * @brief returning current configuration keys from this section!
   * Equal to QSettings::childKeys(), but beginGroup(getGroup()) is set!
   * @note Not usable with Database configuration keys!
   */
  const QStringList getCurrentKeys();

  /**
   * @brief Provider API Configuration Keys
   * Predefined configuration parameters for API connections.
   */
  static const QMap<QString, QMetaType::Type> getProviderApiKeys();

  /**
   * @brief where this configuration will saved?
   */
  virtual TabsConfigWidget::ConfigType getType() const = 0;

  /**
   * @brief menu and widget icon
   */
  virtual const QIcon getIcon() const = 0;

  /**
   * @brief menu and widget title
   */
  virtual const QString getTitle() const = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_CONFIGWIDGET_H
