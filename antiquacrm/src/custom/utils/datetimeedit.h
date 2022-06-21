// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DATETIMEEDIT_UTILS_H
#define DATETIMEEDIT_UTILS_H

#include <QCalendarWidget>
#include <QDateTimeEdit>
#include <QLabel>
#include <QObject>
#include <QWidget>

#include <UtilsMain>

class CalendarViewer final : public QCalendarWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit CalendarViewer(QWidget *parent = nullptr);
};

class DateTimeEdit final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  bool readOnly = false;
  QDateTime p_value;
  QString p_displayFormat;
  QString p_outputFormat;
  QLabel *m_info;
  QDateTimeEdit *m_edit;
  CalendarViewer *m_popup;

public Q_SLOTS:
  void dateChanged(const QDate &);
  void dateTimeChanged(const QDateTime &);
  void timeChanged(const QTime &);
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit DateTimeEdit(QWidget *parent = nullptr);
  const QDateTime system();
  void setDisplayFormat(const QString &format = QString("dd.MM.yyyy"));
  void setOutputFormat(const QString &format = QString("CURRENT_TIMESTAMP"));
  void setReadOnly(bool b = false);
  const QVariant value();
  const QVariant dateTime();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

class DateTimeDisplay final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QDateTime curDateTime;
  QDateTimeEdit *m_edit;

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit DateTimeDisplay(QWidget *parent = nullptr);
  const QDate getDate();
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // DATETIMEEDIT_UTILS_H
