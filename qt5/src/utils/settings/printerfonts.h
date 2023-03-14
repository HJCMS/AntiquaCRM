// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PRINTING_FONTS_H
#define PRINTING_FONTS_H

#include <AntiquaInputEdit>
#include <QGroupBox>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QRegularExpression>
#include <QSignalMapper>
#include <QWidget>

/**
 * @brief The Printer Fonts group
 * @ingroup _settings
 */
class PrinterFonts final : public QGroupBox {
  Q_OBJECT

private:
  /**
   * @brief Für die Dialogknöpfe
   */
  QSignalMapper *m_signalMapper;

  /**
   * @brief Wird für QObject::findchild benötigt!
   * Dieser Reguläre Ausdruck wird verwendet um die Eingabe-Objektklassen zu
   * finden.
   */
  const QRegularExpression p_fontPattern = QRegularExpression("^\\w+_font$");

  /**
   * @brief Bereiche
   */
  QLabel *section(const QString &prefix, const QString &title);

  /**
   * @brief Setze QPushButton und ...
   * Registriere die Signale für den Mapper.
   * @param objName - Empfänger Objektname
   */
  QPushButton *button(const QString &objName);

  /**
   * @brief Beschreibung
   */
  QLabel *description(const QString &);

private Q_SLOTS:
  /**
   * @brief Setze font für Kategorie und Label
   */
  void updateSection(QLabel *);

  /**
   * @brief open Font Dialog
   */
  void setSectionFont(const QString &);

public Q_SLOTS:
  void loadFonts(AntiquaCRM::ASettings *);
  void saveFonts(AntiquaCRM::ASettings *);

public:
  explicit PrinterFonts(QWidget *parent = nullptr);
};

#endif // PRINTING_FONTS_H
