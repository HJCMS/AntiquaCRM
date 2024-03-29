// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PLUGIN_LOADER_H
#define ANTIQUACRM_PLUGIN_LOADER_H

#include <AGlobal>
#include <QDir>
#include <QList>
#include <QObject>
#include <QPluginLoader>

namespace AntiquaCRM {

class APluginInterface;

/**
 * @brief APlugin locader class
 * @ingroup AntiquaCRMPlugin
 */
class ANTIQUACRM_LIBRARY APluginLoader : public QPluginLoader {
  Q_OBJECT

private:
  QDir p_dir;
  QStringList p_filter;
  const QStringList findPlugins();

public:
  explicit APluginLoader(QObject *parent = nullptr);
  void setFileName(const QString &name);
  const QString findPlugin(const QString &name);
  const QList<AntiquaCRM::APluginInterface *> pluginInterfaces(QObject *parent);
};

};     // namespace AntiquaCRM
#endif // ANTIQUACRM_PLUGIN_LOADER_H
