// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CDTRACKING_SELECTGENRE_H
#define CDTRACKING_SELECTGENRE_H

#include <QComboBox>
#include <QLineEdit>
#include <QStringList>
#include <QCompleter>
#include <QObject>
#include <QWidget>
#include <QJsonObject>

/**
 * @brief The SelectGenre class
 * @ingroup utils
 */
class SelectGenre : public QComboBox {
  Q_OBJECT

private:
  QLineEdit *m_search;
  QCompleter *m_cpl;

private Q_SLOTS:
  void find(const QString &);

public Q_SLOTS:
  void reload();

public:
  explicit SelectGenre(QWidget *parent = nullptr);
  static const QJsonObject id3Tags();
  const QString getName();
  int getId();
};

#endif // CDTRACKING_SELECTGENRE_H
