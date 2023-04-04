// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_TABS_LOADER_H
#define ANTIQUACRM_TABS_LOADER_H

#include <AGlobal>
#include <QDir>
#include <QList>
#include <QObject>
#include <QPluginLoader>

namespace AntiquaCRM {

class TabsInterface;

class ANTIQUACRM_LIBRARY TabsLoader final : public QPluginLoader {
  Q_OBJECT

private:
  QDir p_dir;

  QStringList p_filter;

public:
  explicit TabsLoader(QObject *parent = nullptr);

  const QStringList getInterfaceList();

  const QString getInterface(const QString &name);

  bool setInterfaceName(const QString &name);

  const QList<AntiquaCRM::TabsInterface *> interfaces(QObject *parent);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_LOADER_H
