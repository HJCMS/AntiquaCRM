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
#include <QSignalMapper>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class TabsConfigWidget
 * @brief Abstract System Configuration Widget
 *
 * This Widget is used to configure loaded Plugins in "Configuration Dialogs".
 *
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsConfigWidget : public QScrollArea {
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
   * @brief get all AntiquaCRM::AInputWidget from Widget
   * @param parent
   */
  QList<AntiquaCRM::AInputWidget *> getInputList(QObject *parent);

  /**
   * @brief Add AntiquaCRM::AInputWidget to SignalMapper
   *
   * This function register from all AntiquaCRM::AInputWidget the
   * sendInputChanged Signal.
   *
   * You must call this manually!
   * For example in loadSectionConfig when all AntiquaCRM::AInputWidget filled!
   */
  void registerInputChangeSignals();

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
   * @param group  - Configuration TreeView group
   * @param id     - Configuration Identifier group
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

#endif // ANTIQUACRM_TABS_CONFIGWIDGET_H
