// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERBUYERINFO_H
#define ANTIQUACRM_PROVIDERBUYERINFO_H

#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QSplitter>
#include <QTextEdit>
#include <QWidget>

/**
 * @brief The ProviderTextBlock class
 * @ingroup AntiquaProviders
 */
class ProviderTextBlock final : public QFrame {
  Q_OBJECT

private:
  QLabel *m_info;
  QTextEdit *m_edit;

public Q_SLOTS:
  void setContent(const QString &str);

public:
  explicit ProviderTextBlock(const QString &title, QWidget *parent = nullptr);
};

/**
 * @brief The ProviderBuyerInfo class
 * @ingroup AntiquaProviders
 */
class ProviderBuyerInfo final : public QSplitter {
  Q_OBJECT

private:
  ProviderTextBlock *m_invoice;
  ProviderTextBlock *m_comment;

public:
  explicit ProviderBuyerInfo(QWidget *parent = nullptr);
  void setAddress(const QString &str);
  void setBuyerComment(const QString &str);
};

#endif // ANTIQUACRM_PROVIDERBUYERINFO_H
