// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

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
 * @ingroup plugins
 */
class ANTIQUACRM_LIBRARAY APluginLoader : public QPluginLoader {
  Q_OBJECT

private:
  QDir p_dir;
  QStringList p_filter;
  const QStringList findPlugins();

public:
  explicit APluginLoader(QObject *parent = nullptr);
  void setFileName(const QString &name);
  const QString findPlugin(const QString &name);
  const QList<APluginInterface *> pluginInterfaces(QObject *parent);
};

};     // namespace AntiquaCRM
#endif // ANTIQUACRM_PLUGIN_LOADER_H
