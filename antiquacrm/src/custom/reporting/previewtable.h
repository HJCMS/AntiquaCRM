// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PREVIEWTABLE_H
#define PREVIEWTABLE_H

#include <QObject>
#include <QTableView>
#include <QHeaderView>
#include <QWidget>

#include <SqlCore>

class PreviewTableModel;

class PreviewTable final : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int calc_section = -1;
  HJCMS::SqlCore *m_sql;
  PreviewTableModel *m_model;
  QHeaderView *m_tableHeader;

public Q_SLOTS:
  void setQuery(const QString &query);

public:
  explicit PreviewTable(QWidget *parent = nullptr);
  const QString headerName(const QString &key);
  const QString dataHeader(const QString &delimiter = QString(";"));
  const QStringList dataRows(const QString &delimiter = QString(";"));
  const QString salesVolume();
};

#endif // PREVIEWTABLE_H
