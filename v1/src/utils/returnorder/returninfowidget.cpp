// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "returninfowidget.h"

#include <QLayout>
#include <QSizePolicy>

InfoLabel::InfoLabel(const QString &text, QWidget *parent) : QLabel{parent} {
  setWordWrap(true);
  setWordWrap(true);
  setIndent(5);
  setOpenExternalLinks(false);
  setTextFormat(Qt::PlainText);
  setText(text);
}

ReturnInfoWidget::ReturnInfoWidget(QWidget *parent) : QWidget{parent} {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 5);

  QLabel *lbTitle = new QLabel("<b>" + tr("Some notices") + "</b>", this);
  lbTitle->setIndent(5);
  layout->addWidget(lbTitle);

  QStringList l;
  l << tr("A return is only necessary if the order was already settled in the "
          "previous month!");
  l << tr("If the order is not paid or settled, a simple cancellation is "
          "sufficient!");
  l << tr("The return will be booked back in the current month, the "
          "statement from the previous month will not be changed.");
  l << tr("Before you continue, please make sure that this order has already "
          "been booked!");
  l << tr("If you are sure, you can continue now.");
  foreach (QString s, l) {
    layout->addWidget(new InfoLabel(s, this));
  }

  m_ok = new QPushButton(tr("further"), this);
  m_ok->setIcon(QIcon("://icons/action_ok.png"));

  QHBoxLayout *btnLayout = new QHBoxLayout;
  btnLayout->setContentsMargins(5, 5, 5, 5);
  btnLayout->addStretch(1);
  btnLayout->addWidget(m_ok);
  btnLayout->addStretch(1);
  layout->addLayout(btnLayout);
  layout->addStretch(1);
  setLayout(layout);
  connect(m_ok, SIGNAL(clicked()), SIGNAL(sendConfirm()));
}
