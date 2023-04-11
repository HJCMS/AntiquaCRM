// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_INTERFACE_H
#define ANTIQUACRM_TABS_INTERFACE_H

#include <AGlobal>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

class TabsIndex;

class ANTIQUACRM_LIBRARY TabsInterface : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Interface", ANTIQUACRM_TABS_INTERFACE)
  Q_CLASSINFO("Description", "AntiquaCRM TabWidgets Interface")
  Q_CLASSINFO("Version", ANTIQUACRM_VERSION)
  Q_CLASSINFO("Url", ANTIQUACRM_HOMEPAGE)

public:
  explicit TabsInterface(QObject *parent = nullptr);

  virtual const QString displayName() const = 0;

  virtual const QString interfaceName() const = 0;

  virtual const QString sqlTableName() const = 0;

  virtual const QString sqlFieldPrefix() const = 0;

  virtual QWidget *configWidget(QWidget *parent) const = 0;

  virtual bool addIndexOnInit() const = 0;

  virtual AntiquaCRM::TabsIndex *indexWidget(QWidget *parent) const = 0;

  virtual bool createInterface(QObject *parent) = 0;
};

} // namespace AntiquaCRM

Q_DECLARE_INTERFACE(AntiquaCRM::TabsInterface, ANTIQUACRM_TABS_INTERFACE)

#endif // ANTIQUACRM_TABS_INTERFACE_H
