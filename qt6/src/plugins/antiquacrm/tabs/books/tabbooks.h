// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKS_H
#define ANTIQUACRM_PLUGIN_BOOKS_H

#include <AntiquaTabs>
#include <QWidget>
#include <QObject>

class ANTIQUACRM_LIBRARY TabBooks final : public AntiquaCRM::TabsInterface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACRM_TABS_INTERFACE FILE "books.json")
  Q_INTERFACES(AntiquaCRM::TabsInterface)

public:
  explicit TabBooks(QObject *parent = nullptr);

  virtual const QString displayName() const override;

  virtual const QString interfaceName() const override;

  virtual const QString sqlTableName() const override;

  virtual const QString sqlFieldPrefix() const override;

  virtual QWidget *configWidget(QWidget *parent) const override;

  virtual AntiquaCRM::TabsIndex *indexWidget(QWidget *parent) const override;

  virtual bool createInterface(QObject *parent) override;
};

#endif // ANTIQUACRM_PLUGIN_BOOKS_H
