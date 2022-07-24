// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef KEYWORD_LABEL_H
#define KEYWORD_LABEL_H

#include <QLabel>
#include <QObject>
#include <QPaintEvent>
#include <QWidget>

class KeywordLabel final : public QLabel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  // void paintEvent(QPaintEvent *) override;

public:
  explicit KeywordLabel(const QString &txt, QWidget *parent = nullptr);
};

#endif // KEYWORD_LABEL_H
