// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PRINTING_LAYOUT_H
#define PRINTING_LAYOUT_H

#include <AntiquaInputEdit>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QMarginsF>
#include <QObject>
#include <QSlider>
#include <QWidget>

/**
 * @brief The BorderPrintInput class
 * @ingroup _settings
 */
class BorderPrintInput final : public InputEdit {
  Q_OBJECT

private:
  QDoubleSpinBox *m_box;
  QSlider *m_slider;
  void loadDataset() override{/* unused */};

private Q_SLOTS:
  void borderChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setFocus() override;
  void setValue(const QVariant &) override;

public:
  explicit BorderPrintInput(QWidget *parent = nullptr, qreal max = 100);
  void setProperties(const QSqlField &) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

/**
 * @brief The PrintingLayout class
 * @ingroup _settings
 */
class PrintingLayout final : public QGroupBox {
  Q_OBJECT

private:
  /**
   * @brief Tableborder left
   */
  BorderPrintInput *m_left;

  /**
   * @brief Tableborder right
   */
  BorderPrintInput *m_right;

  /**
   * @brief Document Border top, bottom
   */
  qreal p_top, p_bottom;

  /**
   * @brief Document Subject distance to Header
   */
  BorderPrintInput *m_subject;

  /**
   * @brief Body distance to Subject
   */
  BorderPrintInput *m_body;

  /**
   * @brief Empfänger Adresse
   */
  QSlider *m_recipient;

  QLabel *label(const QString &);

public Q_SLOTS:
  void setValue(const QMarginsF &);
  void setValue(qreal, qreal, qreal, qreal);

public:
  explicit PrintingLayout(QWidget *parent = nullptr);
  const QMarginsF value();
  void setSubjectPosition(int);
  int getSubjectPosition();
  void setBodyPosition(int);
  int getBodyPosition();
  void setRecipientPosition(qreal);
  qreal getRecipientPosition();
};

#endif // PRINTING_LAYOUT_H
