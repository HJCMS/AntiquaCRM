// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRIVATE_RUBBERBAND_H
#define ANTIQUACRM_PRIVATE_RUBBERBAND_H

#include <AGlobal>
#include <QGraphicsView>
#include <QObject>
#include <QPaintEvent>
#include <QPen>
#include <QRubberBand>
#include <QStyleOptionRubberBand>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY RubberBand final : public QRubberBand {
  Q_OBJECT

private:
  static const QStyleOptionRubberBand styleOption();
  static const QPen pen();

protected:
  void paintEvent(QPaintEvent *) override;

public Q_SLOTS:
  void reset();

public:
  /**
   * @brief RubberBand for cutting
   */
  explicit RubberBand(QGraphicsView *parent = nullptr);
  bool isValid();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRIVATE_RUBBERBAND_H
