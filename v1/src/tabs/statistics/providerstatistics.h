// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERSTATISTICS_H
#define ANTIQUACRM_PROVIDERSTATISTICS_H

#include <AntiquaCRM>
#include <QDate>
#include <QObject>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

class ProviderStatistics : public QScrollArea {
  Q_OBJECT

private:
  const QDate p_date;
  QString str_year;
  AntiquaCRM::ASqlCore *m_sql;
  QWidget *m_widget;
  QVBoxLayout *m_layout;
  void setOrdersChart();

private Q_SLOTS:
  void highlight(bool);

public:
  explicit ProviderStatistics(const QDate &date, QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PROVIDERSTATISTICS_H
