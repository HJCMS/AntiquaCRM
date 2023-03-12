// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PRINTING_SETUP_H
#define PRINTING_SETUP_H

#include <ASettings>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QObject>
#include <QPair>
#include <QWidget>

/**
 * @brief The Printer Setup class
 * @ingroup _settings
 */
class PrinterSetup : public QGroupBox {
  Q_OBJECT

private:
  QComboBox *m_mainPrinter;
  QComboBox *m_slavePrinter;
  QLabel *label(const QString &);

public:
  explicit PrinterSetup(QWidget *parent = nullptr);

  /**
   * @brief Drucker für Auswahlboxen finden
   */
  void init(AntiquaCRM::ASettings *config);

  /**
   * @brief Standard Drucker für DIN A4
   */
  const QString mainPrinter();

  /**
   * @brief Drucker für DIN A6
   */
  const QString slavePrinter();

  /**
   * @brief Drucker
   * @return QPair<Main,Slave>
   */
  const QPair<QString, QString> printers();
};

#endif // PRINTING_SETUP_H
