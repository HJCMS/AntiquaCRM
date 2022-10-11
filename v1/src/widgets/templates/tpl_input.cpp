// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "@INCLUDE@.h"

#include <QDebug>

@CLASSNAME@::@CLASSNAME@(QWidget *parent) : InputEdit{parent} {

/*
  ->setToolTip(tr(""));
  m_layout->addWidget();
*/

  loadDataset();

  setRequired(true);

}

void @CLASSNAME@::loadDataset() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

void @CLASSNAME@::dataChanged(int) {
  setModified(true);
}

void @CLASSNAME@::reset() {
  setModified(false);
}

void @CLASSNAME@::setValue(const QVariant &val) {
  qDebug() << Q_FUNC_INFO << "TODO" << val;
}

void @CLASSNAME@::setFocus() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

const QVariant @CLASSNAME@::value() {
  qDebug() << Q_FUNC_INFO << "TODO";
  return QVariant();
}

bool @CLASSNAME@::isValid() {
  if (isRequired() && value().isNull())
    return false;

  return true;
}

void @CLASSNAME@::setInfo(const QString &info) {
  setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString @CLASSNAME@::info() { return toolTip(); }

const QString @CLASSNAME@::notes() {
  return tr("This Input is required.");
}
