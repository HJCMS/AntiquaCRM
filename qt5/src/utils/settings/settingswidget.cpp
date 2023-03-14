// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "settingswidget.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget{parent} {
  setObjectName("settingswidget");
  config = new AntiquaCRM::ASettings(this);
}

const QString SettingsWidget::getDirectory(const QString &dest) {
  QString start = (dest.isEmpty()) ? QDir::homePath() : dest;
  QString title = tr("Directory");
  QString d = QFileDialog::getExistingDirectory(this, title, start);
  return (d.isEmpty()) ? QString() : d;
}

const QIcon SettingsWidget::getIcon(const QString &name) {
  return QIcon(":icons/" + name + ".png");
}

void SettingsWidget::chieldModified(bool b) {
  // qDebug() << Q_FUNC_INFO << sender()->objectName() << b;
  if (b) {
    emit pageModified(b);
  }
  setWindowModified(b);
}

void SettingsWidget::chieldChanged(int) { chieldModified(true); }
