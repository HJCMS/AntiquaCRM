// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef KEYWORD_LABEL_H
#define KEYWORD_LABEL_H

#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QToolButton>
#include <QWidget>

/**
 * @ingroup _keywordedit
 * @brief Keywords Edit Label
 */
class KeywordLabel final : public QFrame {
  Q_OBJECT

private:
  const int padding = 2;
  QLabel *m_text;
  QToolButton *m_close;
  const QString lableStyleSheet() const;

private Q_SLOTS:
  void removeClicked();

Q_SIGNALS:
  void sendPleaseRemove(QFrame *);

public:
  explicit KeywordLabel(const QString &txt, QWidget *parent = nullptr);
  const QString text();
};

#endif // KEYWORD_LABEL_H
