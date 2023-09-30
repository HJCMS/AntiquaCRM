// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PUBLISHERS_INPUT_H
#define PUBLISHERS_INPUT_H

#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QWidget>

/**
 * @class PublishersInput
 * @brief Publisher input edit mask
 */
class PublishersInput final : public QWidget {
  Q_OBJECT

private:
  QLineEdit *m_publisher;
  QLineEdit *m_location;
  QLabel *infoCell(const QString &title);

  /**
   * @brief Remove invalid String characters
   */
  const QString strip(const QString &) const;

private Q_SLOTS:
  /**
   * @brief Route WindowModified
   */
  void dataChanged();

public Q_SLOTS:
  /**
   * @brief Clear Input
   */
  void clear();

  /**
   * @brief set Publisher name
   * @param name - e.g. company name
   */
  void setPublisher(const QString &name);

  /**
   * @brief set Location
   * @param location - country/location name
   */
  void setLocation(const QString &location);

public:
  /**
   * @param parent - object
   */
  explicit PublishersInput(QWidget *parent = nullptr);

  /**
   * @brief get Publisher name
   */
  const QString getPublisher();

  /**
   * @brief get Publisher location
   */
  const QString getLocation();
};

#endif // PUBLISHERS_INPUT_H
