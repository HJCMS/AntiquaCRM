// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "popupopenexists.h"

#include <AntiquaWidgets>
#include <QAbstractButton>

PopUpOpenExists::PopUpOpenExists(QWidget *parent) : QMessageBox{parent} {
  setWindowTitle(tr("Order already exists!"));
  setTextFormat(Qt::RichText);
  setStandardButtons(QMessageBox::Open | QMessageBox::No | QMessageBox::Yes);
  QAbstractButton *btn = button(QMessageBox::Yes);
  btn->setIcon(AntiquaCRM::antiquaIcon("action-add"));
  btn->setText(tr("Create"));
  setDefaultButton(QMessageBox::Open);
  setEscapeButton(QMessageBox::No);
}

void PopUpOpenExists::setMessage(const QStringList &data) {
  QString _question("<b>");
  _question.append(tr("Preventing duplicate orders."));
  _question.append("</b>");
  _question.append("<p>");
  _question.append(data.join("</p><p>"));
  _question.append("<p>");
  _question.append(tr("Would you like to open this order?"));
  _question.append("</p>");
  setText(_question);
}

int PopUpOpenExists::exec() {
  if (text().isEmpty())
    return QMessageBox::No;

  return QMessageBox::exec();
}
