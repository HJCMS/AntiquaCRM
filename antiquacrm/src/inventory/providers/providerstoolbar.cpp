// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerstoolbar.h"
#include "myicontheme.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QMenu>
#include <QStringList>
#include <QStyle>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>

ProvidersToolBar::ProvidersToolBar(QWidget *parent) : QFrame{parent} {
  setObjectName("inventory_providers_toolbar");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setObjectName("providers_statusbar_layout");

  m_status = new QLabel(this);
  m_status->setTextFormat(Qt::RichText);
  layout->addWidget(m_status);
  layout->addStretch(1);

  btn_order = new QPushButton(myIcon("db_add"), tr("Create Order"), this);
  QString oInfo = tr("first open the customer before an order can be placed.");
  btn_order->setToolTip(oInfo);
  btn_order->setStatusTip(oInfo);
  btn_order->setEnabled(false);
  layout->addWidget(btn_order);
  btn_refresh = new QPushButton(myIcon("reload"), tr("Refresh"), this);
  QString refreshTip = tr("refreshing orders list");
  btn_refresh->setToolTip(refreshTip);
  btn_refresh->setStatusTip(refreshTip);
  layout->addWidget(btn_refresh);
  setLayout(layout);

  connect(btn_order, SIGNAL(clicked()), this, SIGNAL(s_createOrder()));
  connect(btn_refresh, SIGNAL(clicked()), this, SIGNAL(s_refresh()));

  timerId = startTimer(1000, Qt::PreciseTimer);
}

const QString ProvidersToolBar::timeString() const {
  QTime t = QTime::currentTime();
  return t.toString("hh:mm");
}

void ProvidersToolBar::timerEvent(QTimerEvent *event) {
  --counter;
  if (counter <= 0) {
    counter = 300;
    emit s_refresh();
    return;
  }
  QTime t(0, 0, 0);
  QString btn_text(tr("Refresh") + " ");
  btn_text.append(t.addSecs(counter).toString("m:ss"));
  btn_refresh->setText(btn_text);
}

void ProvidersToolBar::statusMessage(const QString &msg) {
  QString txt("<i>");
  txt.append(timeString() + " ");
  txt.append(msg.trimmed());
  txt.append("</i>");
  m_status->setText(txt);
}

void ProvidersToolBar::warningMessage(const QString &msg) {
  QString txt("<i style='color:darkRed;'>");
  txt.append(timeString() + " ");
  txt.append(msg.trimmed());
  txt.append("</i>");
  m_status->setText(txt);
}

void ProvidersToolBar::enableOrderButton(bool b) { btn_order->setEnabled(b); }
