#include "settingswidget.h"

#include <QDir>
#include <QDebug>
#include <QFileDialog>

SettingsWidget::SettingsWidget(QWidget *parent)
    : QWidget{parent}, Section(QString()) {
  setObjectName("settingswidget");
  config = new ApplSettings(this);
}

const QString SettingsWidget::getDirectory(const QString &dest) {
  QString start = (dest.isEmpty()) ? QDir::homePath() : dest;
  QString title = tr("Directory");
  QString d = QFileDialog::getExistingDirectory(this, title, start);
  return (d.isEmpty()) ? QString() : d;
}

void SettingsWidget::setSection(const QString &str) {
  Section = str;
  emit sectionChanged(Section);
}

const QString SettingsWidget::getSection() { return Section; }
