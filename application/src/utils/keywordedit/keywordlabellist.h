// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef KEYWORD_LABEL_LIST_H
#define KEYWORD_LABEL_LIST_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QObject>
#include <QWidget>

class KeywordLabel;

/**
 * @ingroup KeywordsEdit
 * @brief Keywords Edit Labels list
 */
class KeywordLabelList : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QHBoxLayout *m_layout;
  QStringList p_uniqList;

private Q_SLOTS:
  void removeKeyword(QWidget *);

Q_SIGNALS:
  void sendModified(bool);

public Q_SLOTS:
  void clear();
  void addKeyword(const QString &keyword);

public:
  explicit KeywordLabelList(QWidget *parent = nullptr);
  void addKeywords(const QStringList &keywords);
  const QStringList keywords();
};

#endif // KEYWORD_LABEL_LIST_H
