// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "pluginloader.h"
#include "interface.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QLibraryInfo>

#ifndef DEBUG_PLUGINLOADER
#define DEBUG_PLUGINLOADER false
#endif

static const QStringList filter() {
#ifdef Q_OS_LINUX
  return QStringList("*.so");
#else
  return QStringList("*.dll");
#endif
}

namespace Antiqua {
PluginLoader::PluginLoader(QObject *parent) : QPluginLoader{parent} {
  // Local PluginPath
  QString lp(qApp->applicationDirPath());
  lp.append(QDir::separator());
  lp.append("plugins");
  lp.append(QDir::separator());
  lp.append("antiqua");
  // LSB LIBRARY DESTINATION lib${LIB_SUFFIX}/antiqua/plugins
  QString gp(QLibraryInfo::location(QLibraryInfo::LibrariesPath));
  gp.append(QDir::separator());
  gp.append("antiqua");
  gp.append(QDir::separator());
  gp.append("plugins");

  QStringList paths;
  paths << gp;
  paths << lp;
  paths << QLibraryInfo::location(QLibraryInfo::PluginsPath);

  QFileInfo test(QDir::currentPath(), "cmake_install.cmake");
  if (test.exists()) {
    paths << lp + "/abebooks";
    paths << lp + "/whsoft";
    paths << lp + "/booklooker";
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "Developement Plugin build path add...";
#endif
  }

#ifdef ANTIQUA_DEVELOPEMENT
  if (DEBUG_PLUGINLOADER) {
    qDebug() << Q_FUNC_INFO << paths;
  }
#endif

  p_dir.setSearchPaths("plugins", paths);
  p_dir.setNameFilters(filter());
}

const QStringList PluginLoader::findPlugins() {
  /* Verhindern das gleiche Plugins aus mehreren
   * Verzeichnissen dargestellt werden! */
  QStringList unique;
  QStringList plugins; // Liste der Plugins
  foreach (QString path, p_dir.searchPaths("plugins")) {
    p_dir.setPath(path);
    foreach (QString p, p_dir.entryList(QDir::Files, QDir::Name)) {
      QFileInfo info(p);
      if (QLibrary::isLibrary(p) && !unique.contains(info.baseName())) {
        unique << info.baseName();
        plugins << QString("%1%2%3").arg(p_dir.path(), p_dir.separator(), p);
      }
    }
  }
  unique.clear();
  return plugins;
}

void PluginLoader::setTarget(const QDir &dir) {
  p_dir = dir;
  emit targetChanged(p_dir);
}

void PluginLoader::setFileName(const QString &name) {
  QString f = findPlugin(name);
  QPluginLoader::setFileName(f);
}

const QDir PluginLoader::getTarget() { return p_dir; }

const QString PluginLoader::findPlugin(const QString &name) {
  foreach (QString plugin, findPlugins()) {
    if (plugin.contains(name, Qt::CaseInsensitive))
      return plugin;
  }
  return QString();
}

const QList<Interface *> PluginLoader::pluginInterfaces(QObject *parent) {
  QList<Interface *> list;
  foreach (QString file, findPlugins()) {
    setFileName(file);
    QObject *plug = instance();
    if (plug) {
      QJsonObject info = metaData().value("MetaData").toObject();
      QString objName = info.value("Name").toString();
      if (objName.isEmpty()) {
        qWarning("Missing Metadata: %s", qPrintable(file));
        continue;
      }
      Interface *m_Iface = qobject_cast<Interface *>(plug);
      if (m_Iface != nullptr && m_Iface->createInterface(parent)) {
        m_Iface->setObjectName(objName);
        list << m_Iface;
      } else {
        qWarning("Loading '%s' failed.",
                 qPrintable(info.value("Title").toString()));
      }
    } else
      qWarning("(Antiqua) Pluginloader: %s", qPrintable(errorString()));
  }
  return list;
}

}; // namespace Antiqua
