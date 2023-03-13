// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef KEYWORD_LABEL_LIST_H
#define KEYWORD_LABEL_LIST_H

#include <QHBoxLayout>
#include <QObject>
#include <QStringList>
#include <QWidget>

/**
 * @ingroup _keywordedit
 * @brief Keywords Edit Labels list
 */
class KeywordLabelList final : public QWidget {
  Q_OBJECT

private:
  int p_maxLength = 60;
  QHBoxLayout *m_layout;
  QStringList p_uniqList;

private Q_SLOTS:
  /**
   * @brief Schlüsselwort entfernen
   */
  void removeKeyword();

  /**
   * @brief Den toolTip neu generieren!
   * Wird ausgelöst wenn sich die Schlüsselwortlänge ändert!
   */
  void updateToolTip();

Q_SIGNALS:
  void sendModified(bool);

public Q_SLOTS:
  void clear();
  void setMaxLength(int);
  void addKeyword(const QString &keyword);

public:
  explicit KeywordLabelList(QWidget *parent = nullptr);
  void addKeywords(const QStringList &keywords);
  int maxLength();
  const QStringList keywords();
};

#endif // KEYWORD_LABEL_LIST_H
