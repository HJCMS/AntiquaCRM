// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "apluginloader.h"
#include "aplugininterface.h"
#include "asettings.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QLibraryInfo>
#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#define ANTIQUA_DEBUG_PLUGINS true
#endif

namespace AntiquaCRM {

APluginLoader::APluginLoader(QObject *parent) : QPluginLoader{parent} {
  p_dir = ASettings::getPluginDir();
  p_filter = ASettings::pluginSearchFilter();
}

const QStringList APluginLoader::findPlugins() {
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
#ifdef ANTIQUA_DEBUG_PLUGINS
  if (plugins.isEmpty()) {
    qDebug() << Q_FUNC_INFO << "Missing Plugins:" << p_dir.path();
  }
#endif
  return plugins;
}

void APluginLoader::setFileName(const QString &name) {
  QString f = findPlugin(name);
  QPluginLoader::setFileName(f);
}

const QString APluginLoader::findPlugin(const QString &name) {
  foreach (QString plugin, findPlugins()) {
    if (plugin.contains(name, Qt::CaseInsensitive))
      return plugin;
  }
  return QString();
}

const QList<APluginInterface *>
APluginLoader::pluginInterfaces(QObject *parent) {
  QList<APluginInterface *> list;
  foreach (QString file, findPlugins()) {
    setFileName(file);
    QObject *m_plugin = instance();
    if (m_plugin) {
      QJsonObject info = metaData().value("MetaData").toObject();
      QString objName = info.value("Name").toString();
      if (objName.isEmpty()) {
        qWarning("PluginLoader missing Metadata: %s", qPrintable(file));
        continue;
      }

      APluginInterface *m_iface = qobject_cast<APluginInterface *>(m_plugin);
      if (m_iface != nullptr && m_iface->createInterface(parent)) {
        m_iface->setObjectName(objName);
        list << m_iface;
      } else {
        qWarning("Plugin loading for '%s' failed.",
                 qPrintable(info.value("Name").toString()));
      }
    } else {
      qWarning("AntiquaCRM::APluginLoader: %s", qPrintable(errorString()));
    }
  }
  return list;
}

}; // namespace AntiquaCRM
