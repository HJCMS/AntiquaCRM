// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DATERANGEWIDGET_H
#define DATERANGEWIDGET_H

#include <QCalendarWidget>
#include <QLabel>
#include <QObject>
#include <QWidget>

class DateRangeWidget : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QCalendarWidget *m_fromDate;
  QCalendarWidget *m_toDate;
  QLabel *setLabel(const QString &);

public:
  explicit DateRangeWidget(QWidget *parent = nullptr);
  const QPair<QDate,QDate> getDateRange();
  const QPair<QString,QString> timestampRange();
};

#endif // DATERANGEWIDGET_H
