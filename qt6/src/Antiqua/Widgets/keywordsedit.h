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

class KeywordListView;

/**
 * @class KeywordsEdit
 * @brief Text Input for Keywords
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY KeywordsEdit final : public AntiquaCRM::AbstractInput {
  Q_OBJECT

private:
  /**
   * @brief Start auto-completion only from minimum length
   */
  const int minLength = 3;

  /**
   * @todo Displays the current keywords.
   */
  KeywordListView *m_keywords;

  /**
   * @brief input box
   */
  ALineEdit *m_edit;

private Q_SLOTS:
  /**
   * @brief Default input monitoring
   */
  void valueChanged();

  /**
   * @brief Remove all keywords with Popup Warning!
   */
  void clearKeywords();

  /**
   * @brief Insert keyword from ALineEdit.
   */
  void insertKeyword();

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
