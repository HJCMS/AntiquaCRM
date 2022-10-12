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

class ANTIQUACRM_LIBRARAY APluginLoader : public QPluginLoader {
  Q_OBJECT
  Q_PROPERTY(QDir target READ getTarget WRITE setTarget NOTIFY targetChanged)

private:
  QDir p_dir;
  const QStringList findPlugins();

Q_SIGNALS:
  void targetChanged(const QDir &dir);

public Q_SLOTS:
  void setTarget(const QDir &dir);

public:
  explicit APluginLoader(QObject *parent = nullptr);
  void setFileName(const QString &name);
  const QDir getTarget();
  const QString findPlugin(const QString &name);
  const QList<APluginInterface *> pluginInterfaces(QObject *parent);
};

};     // namespace AntiquaCRM
#endif // ANTIQUACRM_PLUGIN_LOADER_H
