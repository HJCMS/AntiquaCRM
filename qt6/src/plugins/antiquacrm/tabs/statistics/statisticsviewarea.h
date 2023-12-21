// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_VIEWAREA_H
#define ANTIQUACRM_STATISTICS_VIEWAREA_H

#include <AGlobal>
#include <QObject>
#include <QPaintEvent>
#include <QScrollArea>
#include <QWidget>

class ANTIQUACRM_LIBRARY StatisticsViewArea final : public QScrollArea {
  Q_OBJECT

private:
  inline const QString infoText() const;
  virtual void paintEvent(QPaintEvent *) override;

public:
  explicit StatisticsViewArea(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_STATISTICS_VIEWAREA_H
