// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "helperdialog.h"

#include <QDebug>

HelperDialog::HelperDialog(QWidget *parent) : QDialog{parent, Qt::Tool} {
  layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_dummy = new QWidget(this);
  layout->insertWidget(0, m_dummy);

  btn_ok = new QPushButton(tr("Ok"), this);
  layout->insertWidget(1, btn_ok);

  layout->setStretch(0, 1);
  setLayout(layout);

  connect(btn_ok, SIGNAL(clicked()), SLOT(accept()));
}

void HelperDialog::setHelp(QWidget *w) {
  if (w != nullptr) {
    layout->replaceWidget(m_dummy, w, Qt::FindDirectChildrenOnly);
    update();
  }
}

int HelperDialog::exec() {
  qDebug() << Q_FUNC_INFO << "Fallback";
  return QDialog::exec();
}

int HelperDialog::exec(const QString &section) {
  qDebug() << Q_FUNC_INFO << section;
  return QDialog::exec();
}

HelperDialog::~HelperDialog() {}
