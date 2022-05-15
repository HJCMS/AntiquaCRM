#include "settingswidget.h"
#include "applsettings.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget{parent} {
  setObjectName("settingswidget");
  config = new ApplSettings(this);
}

const QString SettingsWidget::getDirectory(const QString &dest) {
  QString start = (dest.isEmpty()) ? QDir::homePath() : dest;
  QString title = tr("Directory");
  QString d = QFileDialog::getExistingDirectory(this, title, start);
  return (d.isEmpty()) ? QString() : d;
}

void SettingsWidget::chieldModified(bool b) {
  if (b) {
    emit pageModified(b);
  }
  setWindowModified(b);
}

void SettingsWidget::chieldChanged(int) { chieldModified(true); }
