// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "pathsettings.h"

PathSettings::PathSettings(QWidget *parent) : SettingsWidget{parent} {
  setObjectName("path_settings");

  m_layout = new QGridLayout(this);
  m_layout->setObjectName("path_config_layout");

  int lrow = 0;
  Qt::Alignment t_align = (Qt::AlignVCenter | Qt::AlignRight);

  QString info_txt = tr("Directory Configuration");
  QLabel *info_label = new QLabel(info_txt, this);
  m_layout->addWidget(info_label, lrow++, 0, 1, 3);

  // Bilderarchiv
  m_layout->addWidget(infoCell(tr("Images")), lrow, 0, 1, 1, t_align);
  m_images_path = new QLineEdit(this);
  m_images_path->setObjectName("images");
  m_layout->addWidget(m_images_path, lrow, 1, 1, 1);
  m_layout->addWidget(setPathBtn("images"), lrow++, 2, 1, 1);

  // Lieferscheine
  m_layout->addWidget(infoCell(tr("Deliverynotes")), lrow, 0, 1, 1, t_align);
  m_delivery_path = new QLineEdit(this);
  m_delivery_path->setObjectName("deliverynotes");
  m_layout->addWidget(m_delivery_path, lrow, 1, 1, 1);
  m_layout->addWidget(setPathBtn("deliverynotes"), lrow++, 2, 1, 1);

  // Rechnungen
  m_layout->addWidget(infoCell(tr("Invoices")), lrow, 0, 1, 1, t_align);
  m_invoice_path = new QLineEdit(this);
  m_invoice_path->setObjectName("invoices");
  m_layout->addWidget(m_invoice_path, lrow, 1, 1, 1);
  m_layout->addWidget(setPathBtn("invoices"), lrow++, 2, 1, 1);

  // Berichte
  m_layout->addWidget(infoCell(tr("Reports")), lrow, 0, 1, 1, t_align);
  m_reports_path = new QLineEdit(this);
  m_reports_path->setObjectName("reports");
  m_layout->addWidget(m_reports_path, lrow, 1, 1, 1);
  m_layout->addWidget(setPathBtn("reports"), lrow++, 2, 1, 1);

  // Erinnerungen
  m_layout->addWidget(infoCell(tr("Reminder")), lrow, 0, 1, 1, t_align);
  m_reminder_path = new QLineEdit(this);
  m_reminder_path->setObjectName("reminder");
  m_layout->addWidget(m_reminder_path, lrow, 1, 1, 1);
  m_layout->addWidget(setPathBtn("reminder"), lrow++, 2, 1, 1);

  // Buchkarten
  m_layout->addWidget(infoCell(tr("Bookcards")), lrow, 0, 1, 1, t_align);
  m_bookcard_path = new QLineEdit(this);
  m_bookcard_path->setObjectName("cards");
  m_layout->addWidget(m_bookcard_path, lrow, 1, 1, 1);
  m_layout->addWidget(setPathBtn("cards"), lrow++, 2, 1, 1);

  // END

  m_layout->setRowStretch(lrow, 1);
  setLayout(m_layout);
}

QLabel *PathSettings::infoCell(const QString &title) {
  QLabel *lb = new QLabel(title + ":", this);
  return lb;
}

QToolButton *PathSettings::setPathBtn(const QString &txName) {
  QToolButton *lb = new QToolButton(this);
  lb->setObjectName("open_" + txName);
  lb->setIcon(QIcon("://icons/view_list.png"));
  lb->setToolTip(tr("Open Directory"));
  connect(lb, SIGNAL(clicked()), SLOT(openDirsDialog()));
  return lb;
}

void PathSettings::openDirsDialog() {
  QObject *m_sender = sender();
  if (m_sender == nullptr)
    return;

  QString name = m_sender->objectName().replace("open_", "").trimmed();
  QLineEdit *edit = findChild<QLineEdit *>(name, p_options);
  if (edit == nullptr)
    return;

  QString old_path = edit->text();
  old_path = old_path.isEmpty() ? QDir::homePath() : old_path;
  QVariant spath = config->value("dirs/" + name, old_path);
  QString src = getDirectory(spath.toString());
  if (!src.isEmpty()) {
    edit->setText(src);
  }
}

void PathSettings::setPageTitle(const QString &title) {
  pageTitle = title;
  emit pageTitleChanged();
}

const QString PathSettings::getPageTitle() { return pageTitle; }

void PathSettings::setPageIcon(const QIcon &icon) {
  pageIcon = icon;
  emit pageIconChanged();
}

const QIcon PathSettings::getPageIcon() { return pageIcon; }

void PathSettings::loadSectionConfig() {
  QString n;
  config->beginGroup("dirs");
  QList<QLineEdit *> l = findChildren<QLineEdit *>(n, p_options);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      QLineEdit *w = l.at(i);
      if (w != nullptr && config->contains(w->objectName())) {
        w->setText(config->value(w->objectName()).toString());
      }
    }
  }
  config->endGroup();
}

void PathSettings::saveSectionConfig() {
  QString n;
  config->beginGroup("dirs");
  QList<QLineEdit *> l = findChildren<QLineEdit *>(n, p_options);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      QLineEdit *w = l.at(i);
      if (w != nullptr) {
        config->setValue(w->objectName(), w->text());
      }
    }
  }
  config->endGroup();
}
