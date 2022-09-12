// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_PLUGINLOADER_H
#define ANTIQUA_PLUGINLOADER_H

#include <AntiquaCRM>
#include <QDir>
#include <QList>
#include <QObject>
#include <QPluginLoader>

namespace Antiqua {
  class Interface;
  class ANTIQUACORE_EXPORT PluginLoader : public QPluginLoader {
    Q_OBJECT
    Q_CLASSINFO("Author", "Jürgen Heinemann")
    Q_CLASSINFO("URL", "https://www.hjcms.de")
    Q_PROPERTY(QDir target READ getTarget WRITE setTarget NOTIFY targetChanged)

  private:
    QDir p_dir;
    const QStringList findPlugins();

  Q_SIGNALS:
    void targetChanged(const QDir &);

  public Q_SLOTS:
    void setTarget(const QDir &);

  public:
    explicit PluginLoader(QObject *parent = nullptr);
    void setFileName(const QString &name);
    const QDir getTarget();
    const QString findPlugin(const QString &name);
    const QList<Interface*> pluginInterfaces(QObject * parent);
  };
};     // namespace Antiqua
#endif // ANTIQUA_PLUGINLOADER_H
