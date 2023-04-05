// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_KEYWORDSEDIT_H
#define ANTIQUACRM_WIDGETS_KEYWORDSEDIT_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class KeywordsEdit
 * @brief Text Input for Keywords
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY KeywordsEdit final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  /**
   * @brief Trennzeichen
   */
  const QChar p_delimiter = QChar(',');

  /**
   * @brief Autovervolständigung erst ab Mindestlänge starten
   */
  const int minLength = 3;

  /**
   * @todo Update needet
   */
  QWidget *m_keysList;

  /**
   * @brief Eingabefeld
   */
  ALineEdit *m_lineEdit;

private Q_SLOTS:
  /**
   * @brief Liste leeren
   * Beinhaltet ein Warn PopUp
   */
  void clearKeywords();

  /**
   * @brief Text vom Completer Signal aufarbeiten
   * An dieser Stelle mit dem Puffer von @ref p_keywords das ganze wieder
   * zusammenfügen.
   */
  void finalize();

  void valueChanged();

public Q_SLOTS:
  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @param parent - parent widget
   */
  explicit KeywordsEdit(QWidget *parent = nullptr);

  void initData() override;

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_KEYWORDSEDIT_H
