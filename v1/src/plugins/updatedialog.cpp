// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "updatedialog.h"

#include <QDebug>
#include <QJsonValue>
#include <QLabel>
#include <QLayout>
#include <QTimer>

namespace AntiquaCRM {

UpdateDialog::UpdateDialog(QWidget *parent) : QDialog{parent} {
  setSizeGripEnabled(true);
  setContentsMargins(5, 5, 5, 15);
}

}; // namespace AntiquaCRM
