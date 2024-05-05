// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providersloader.h"
#include "profiderinterface.h"

#include <ASettings>

namespace AntiquaCRM {

ProvidersLoader::ProvidersLoader(QObject *parent) : QPluginLoader{parent} {
  AntiquaCRM::ASettings config(this);
  // ANTIQUACRM_PLUGIN_TARGET
  p_dir = config.getPluginDir("providers");
  // Erweiterungen
  p_filter = config.pluginSearchFilter();
}

const QStringList ProvidersLoader::getInterfaceList() {
  QStringList plugins;
  QDir::Filters sFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  foreach (QString p, p_dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
    if (p_dir.cd(p)) {
      foreach (QFileInfo info, p_dir.entryInfoList(p_filter, sFilter)) {
        QString plugFile = info.filePath();
        if (QLibrary::isLibrary(plugFile)) {
          plugins << plugFile;
        }
      }
      p_dir.cdUp();
    }
  }
#ifdef ANTIQUA_DEVELOPMENT
  if (plugins.isEmpty()) {
    qDebug() << Q_FUNC_INFO << "Missing Plugins:" << p_dir.path();
  }
#endif
  return plugins;
}

const QString ProvidersLoader::getInterface(const QString &name) {
  foreach (QString plugin, getInterfaceList()) {
    if (plugin.contains(name, Qt::CaseInsensitive))
      return plugin;
  }
  return QString();
}

bool ProvidersLoader::setInterfaceName(const QString &name) {
  QString _plugin = getInterface(name);
  if (_plugin.isEmpty())
    return false;

  QPluginLoader::setFileName(_plugin);
  return true;
}

const QList<AntiquaCRM::ProviderInterface *>
ProvidersLoader::interfaces(QObject *parent) {
  QList<AntiquaCRM::ProviderInterface *> _list;
  if (parent == nullptr)
    return _list;

  foreach (QString path, getInterfaceList()) {
    QFileInfo info(path);
    if (!info.isFile())
      continue;

    setFileName(path);
    QObject *m_plugin = instance();
    if (m_plugin) {
      QJsonObject info = metaData().value("MetaData").toObject();
      QString objName = info.value("Name").toString();
      if (objName.isEmpty()) {
        qWarning("PluginLoader missing Metadata: %s", qPrintable(path));
        continue;
      }

      AntiquaCRM::ProviderInterface *m_iface =
          qobject_cast<AntiquaCRM::ProviderInterface *>(m_plugin);
      if (m_iface != nullptr && m_iface->createInterface(parent)) {
        m_iface->setObjectName(objName);
        _list << m_iface;
      } else {
        qWarning("Plugin loading for '%s' failed.",
                 qPrintable(info.value("Name").toString()));
      }
    } else {
      qWarning("AntiquaCRM::APluginLoader: %s", qPrintable(errorString()));
    }
  }
  return _list;
}

} // namespace AntiquaCRM
