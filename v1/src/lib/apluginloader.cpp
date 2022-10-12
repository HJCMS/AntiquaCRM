// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "apluginloader.h"
#include "aplugininterface.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QLibraryInfo>
#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#define ANTIQUA_DEBUG_PLUGINS true
#endif

namespace AntiquaCRM {

static const QStringList filter() {
#ifdef Q_OS_LINUX
  return QStringList("*.so");
#else
  return QStringList("*.dll");
#endif
}

APluginLoader::APluginLoader(QObject *parent) : QPluginLoader{parent} {
  setTarget(QDir(ANTIQUACRM_PLUGIN_TARGET));
}

const QStringList APluginLoader::findPlugins() {
  QStringList unique;
  QStringList plugins;
  foreach (QString p, p_dir.entryList(QDir::Files, QDir::Name)) {
    QFileInfo info(p);
    if (QLibrary::isLibrary(p) && !unique.contains(info.baseName())) {
      unique << info.baseName();
      plugins << QString("%1%2%3").arg(p_dir.path(), p_dir.separator(), p);
    }
  }
#ifdef ANTIQUA_DEBUG_PLUGINS
  qDebug() << Q_FUNC_INFO << unique << plugins;
#endif
  unique.clear();
  return plugins;
}

void APluginLoader::setTarget(const QDir &dir) {
  p_dir = dir;
  p_dir.setNameFilters(filter());
  emit targetChanged(p_dir);
}

void APluginLoader::setFileName(const QString &name) {
  QString f = findPlugin(name);
  QPluginLoader::setFileName(f);
}

const QDir APluginLoader::getTarget() { return p_dir; }

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
    QObject *plug = instance();
    if (plug) {
      QJsonObject info = metaData().value("MetaData").toObject();
      QString objName = info.value("Name").toString();
      if (objName.isEmpty()) {
        qWarning("Missing Metadata: %s", qPrintable(file));
        continue;
      }
      APluginInterface *m_Iface = qobject_cast<APluginInterface *>(plug);
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

}; // namespace AntiquaCRM
