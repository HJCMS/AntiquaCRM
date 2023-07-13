// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CONFIGWIDGET_H
#define ANTIQUACRM_PLUGIN_CONFIGWIDGET_H

#include <AntiquaInput>
#include <QIcon>
#include <QMap>
#include <QMetaType>
#include <QObject>
#include <QScrollArea>
#include <QSignalMapper>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class PluginConfigWidget
 * @brief Abstract Plugin Configuration Widget
 *
 * This Abstract Widget is used to configure Plugins in "Configuration Dialogs".
 *
 * There are currently two ways to specify saving with Enumeration "ConfigType".
 *
 * This can do manually in virtual Slots loadSectionConfig and
 * saveSectionConfig.
 *
 * When using Database the PostgreSQL Table "antiquacrm_configs" is used. Field
 * "cfg_group" must a unique Key, the field "cfg_jsconfig" requires a
 * QJsonDokument to store the data.
 *
 * The class constructor need a "config group" and "config id".
 * This will create the config group Section.
 * You can use getGroup() to get the current Group Section and getCurrentKeys()
 * to get all Group Section keys and only usable if ConfigType::CONFIG_SYSTEM
 * is set. All keys and values are without restrictions for Plugin Developers.
 *
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY PluginConfigWidget : public QScrollArea {
  Q_OBJECT

private:
  QSignalMapper *signalMapper;
  const QString p_gid;

private Q_SLOTS:
  void setInputEditChanged(QObject *);

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
   * @brief get InputWidget from Child widget
   * @param parent - parent Widget
   * @param name  - objectName
   */
  AntiquaCRM::AInputWidget *inputWidget(QWidget *parent, const QString &name);

  /**
   * @brief get all AntiquaCRM::AInputWidget from Widget
   * @param parent
   */
  QList<AntiquaCRM::AInputWidget *> getInputList(QObject *parent);

  /**
   * @brief Add AntiquaCRM::AInputWidget to SignalMapper
   * @param base - Read ANTIQUACRM_TABS_CONFIGWIDGET_HQObject::base
   * This function register from all AntiquaCRM::AInputWidget the
   * sendInputChanged Signal.
   *
   * You must call this manually!
   * For example in loadSectionConfig when all AntiquaCRM::AInputWidget filled!
   */
  void registerInputChangeSignals(QObject *base);

public Q_SLOTS:
  /**
   * @brief load operation
   */
  virtual void loadSectionConfig() = 0;

  /**
   * @brief save operation
   */
  virtual void saveSectionConfig() = 0;

public:
  /**
   * @brief Enumeration to know, how to store configs.
   */
  enum ConfigType {
    CONFIG_SYSTEM = 0,  /**< @brief store in user config */
    CONFIG_DATABASE = 1 /**< @brief store in database */
  };

  /**
   * @param group  - Configuration Group Section
   * @param id     - Configuration Identifier
   * @param parent - Parent Widget
   */
  explicit PluginConfigWidget(const QString &group, // config group
                              const QString &id,    // config id
                              QWidget *parent = nullptr);

  ~PluginConfigWidget();

  /**
   * @brief configuration group path
   * @note Not usable with ConfigType::CONFIG_DATABASE!
   */
  const QString getGroup() const;

  /**
   * @brief returning current configuration keys from this section!
   * Equal to QSettings::childKeys(), but beginGroup(getGroup()) is set!
   * @note Not usable with ConfigType::CONFIG_DATABASE!
   */
  const QStringList getCurrentKeys();

  /**
   * @brief where this configuration will saved?
   */
  virtual PluginConfigWidget::ConfigType getType() const = 0;

  /**
   * @brief menu entry data
   * @code
   *  QJsonObject {
   *   "icon" = QString,    // IconTheme baseName
   *   "title" = QString,   // Menu and Widget titel
   *   "tooltip" = QString  // Menu and Widget tooltip
   *  }
   * @endcode
   */
  virtual const QJsonObject getMenuEntry() const = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PLUGIN_CONFIGWIDGET_H
