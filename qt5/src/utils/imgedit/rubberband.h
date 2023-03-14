// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_RUBBERBAND_H
#define ANTIQUA_RUBBERBAND_H

#include <QObject>
#include <QPaintEvent>
#include <QPen>
#include <QRubberBand>
#include <QStyleOptionRubberBand>
#include <QGraphicsView>

/**
 * @ingroup _imgedit
 * @brief RubberBand for cutting
 */
class RubberBand final : public QRubberBand {
  Q_OBJECT

private:
  static const QStyleOptionRubberBand styleOption();
  static const QPen pen();

protected:
  void paintEvent(QPaintEvent *) override;

public Q_SLOTS:
  void reset();

public:
  explicit RubberBand(QGraphicsView *parent = nullptr);
  bool isValid();
};

#endif // ANTIQUA_RUBBERBAND_H
