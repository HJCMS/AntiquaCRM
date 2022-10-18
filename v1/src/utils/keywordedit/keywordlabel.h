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

class KeywordLabel final : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const int padding = 2;
  QLabel *m_text;
  QToolButton *m_close;
  const QString lableStyleSheet() const;

private Q_SLOTS:
  void removeClicked();

Q_SIGNALS:
  void sendPleaseRemove(QWidget *);

public:
  explicit KeywordLabel(const QString &txt, QWidget *parent = nullptr);
  const QString text();
};

#endif // KEYWORD_LABEL_H
