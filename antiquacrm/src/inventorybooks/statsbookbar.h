// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STATSBOOKBAR_H
#define STATSBOOKBAR_H

#include <QtCore/QObject>
#include <QtCore/QTimerEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolBar>

class StatsBookBar : public QToolBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  int timerID;
  QLabel *m_infoLabel;
  QComboBox *m_showHistory;
  void addComboBoxData();

private Q_SLOTS:
  void historyChanged(int);

protected:
  void timerEvent(QTimerEvent *);

Q_SIGNALS:
  void s_queryHistory(const QString &);

public Q_SLOTS:
  void showMessage(const QString &);
  void setThisDayHistory();

public:
  explicit StatsBookBar(QWidget *parent = nullptr);
};

#endif // STATSBOOKBAR_H
