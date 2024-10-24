// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SPLITTER_H
#define ANTIQUACRM_WIDGETS_SPLITTER_H

#include <AntiquaInput>
#include <QObject>
#include <QSplitter>
#include <QSplitterHandle>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class SplitterHandle
 * @brief AntiquaCRM Splitter handle
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY SplitterHandle final : public QSplitterHandle {
  Q_OBJECT

protected:
  void mouseReleaseEvent(QMouseEvent *) override;

Q_SIGNALS:
  void sendSaveState();

public:
  explicit SplitterHandle(QSplitter *parent = nullptr);
};

/**
 * @class Splitter
 * @brief Horizontal Splitter for EditorWidgets
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY Splitter final : public QSplitter {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  QString name = QString();

  /**
   * @brief Create and set configPath with ObjectName
   */
  const QString configPath();

  /**
   * @brief Create the Splitterhandle
   */
  SplitterHandle *createHandle() override;

  /**
   * @brief Load saved state on show/visible
   */
  void showEvent(QShowEvent *) override;

private Q_SLOTS:
  /**
   * @brief saveState() Methode
   */
  void setSaveState();

public:
  /**
   * @param parent - parent object
   */
  explicit Splitter(QWidget *parent = nullptr);

  /**
   * @brief Insert Index 0 with stretch factor 2
   * @param widget - left Widget
   */
  void addLeft(QWidget *widget);

  /**
   * @brief Insert Index 1 with stretch factor 1
   * @param widget - right Widget
   */
  void addRight(QWidget *widget);

  virtual ~Splitter();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SPLITTER_H
