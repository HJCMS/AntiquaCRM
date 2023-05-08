// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printdeliverynote.h"

namespace AntiquaCRM {

PrintDeliveryNote::PrintDeliveryNote(QWidget *parent) : APrintDialog{parent} {}

bool PrintDeliveryNote::generateDocument(QPrinter *printer) {
  Q_UNUSED(printer);
  qDebug() << Q_FUNC_INFO << "TODO";
  return false;
}

void PrintDeliveryNote::openPrintDialog() { qDebug() << Q_FUNC_INFO << "TODO"; }

int PrintDeliveryNote::exec(const QStringList &options) {
  Q_UNUSED(options);
  return QDialog::exec();
}

} // namespace AntiquaCRM
