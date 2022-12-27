// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "returnnavigation.h"

#include <QIcon>

ReturnNavigation::ReturnNavigation(QWidget *parent) : QDialogButtonBox{parent} {
  m_save = addButton(QDialogButtonBox::Apply);
  m_save->setIcon(setIcon("action_save"));
  m_save->setEnabled(false);

  m_cancel = addButton(QDialogButtonBox::Close);
  m_cancel->setIcon(setIcon("action_quit"));

  connect(m_save, SIGNAL(clicked()), SIGNAL(sendSave()));
}

const QIcon ReturnNavigation::setIcon(const QString &name) {
  return QIcon("://icons/" + name + ".png");
}

void ReturnNavigation::enableSaveButton(bool b) { m_save->setEnabled(b); }
