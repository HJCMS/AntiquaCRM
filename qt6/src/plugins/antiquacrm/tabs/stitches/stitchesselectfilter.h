// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_STITCHESSELECTFILTER_H
#define ANTIQUACRM_PLUGIN_STITCHESSELECTFILTER_H

#include <AntiquaTabs>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_LIBRARY StitchesSelectFilter final
    : public AntiquaCRM::TabsSearchBarFilter {
  Q_OBJECT

private:
  virtual void createItemData() override;

public:
  explicit StitchesSelectFilter(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PLUGIN_STITCHESSELECTFILTER_H
