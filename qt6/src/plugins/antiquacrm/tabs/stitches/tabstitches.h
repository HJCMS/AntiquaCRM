// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_STITCHES_H
#define ANTIQUACRM_PLUGIN_STITCHES_H

#include <AntiquaTabs>
#include <QWidget>
#include <QObject>

class ANTIQUACRM_LIBRARY TabStitches final : public AntiquaCRM::TabsInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_TABS_INTERFACE FILE "stitches.json")
  Q_INTERFACES(AntiquaCRM::TabsInterface)

public:
  explicit TabStitches(QObject *parent = nullptr);

  const QString displayName() const override;

  const QString interfaceName() const override;

  const QString sqlTableName() const override;

  const QString sqlFieldPrefix() const override;

  AntiquaCRM::TabsConfigWidget *configWidget(QWidget *parent) const override;

  bool addIndexOnInit() const override;

  AntiquaCRM::TabsIndex *indexWidget(QWidget *parent) const override;

  bool createInterface(QObject *parent) override;
};

#endif // ANTIQUACRM_PLUGIN_STITCHES_H
