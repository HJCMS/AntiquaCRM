// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "companysettings.h"

CompanySettings::CompanySettings(QWidget *parent) : SettingsWidget{parent} {
  setObjectName("company_settings");

  m_layout = new QGridLayout(this);
  m_layout->setObjectName("company_config_layout");

  QString info_txt =
      tr("On this page you configure your company's presentation settings.");
  QLabel *info_label = new QLabel(info_txt, this);
  m_layout->addWidget(info_label, layoutRow++, 0, 1, 2);

  setLayout(m_layout);
}

QLabel *CompanySettings::infoCell(const QString &txt) {
  QLabel *lb = new QLabel(txt + ":", this);
  return lb;
}

void CompanySettings::initSignalChanged() {}

void CompanySettings::lineEditChanged(const QString &) { chieldModified(true); }

void CompanySettings::setPageTitle(const QString &title) {
  pageTitle = title;
  emit pageTitleChanged();
}

const QString CompanySettings::getPageTitle() { return pageTitle; }

void CompanySettings::setPageIcon(const QIcon &icon) {
  pageIcon = icon;
  emit pageIconChanged();
}

const QIcon CompanySettings::getPageIcon() { return pageIcon; }

void CompanySettings::loadSectionConfig() {
  AntiquaCRM::ASqlCore sql(this);
  QSqlQuery q = sql.query("SELECT * FROM antiquacrm_company ORDER BY ac_sort;");
  if (q.size() > 0) {
    while (q.next()) {
      int _type = q.value("ac_edit").toInt();
      if (_type == 1) {
        QLabel *m_info = infoCell(q.value("ac_info").toString());
        m_layout->addWidget(m_info, layoutRow, 0, 1, 1,
                            (Qt::AlignTop | Qt::AlignRight));
        QTextEdit *m_edit = new QTextEdit(this);
        QString ac_class = q.value("ac_class").toString();
        m_edit->setObjectName(ac_class);
        m_edit->setPlaceholderText(ac_class);
        m_edit->setPlainText(q.value("ac_value").toString());
        m_layout->addWidget(m_edit, layoutRow, 1, 1, 1);
      } else {
        QLabel *m_info = infoCell(q.value("ac_info").toString());
        m_layout->addWidget(m_info, layoutRow, 0, 1, 1, Qt::AlignRight);
        QLineEdit *m_edit = new QLineEdit(this);
        QString ac_class = q.value("ac_class").toString();
        m_edit->setObjectName(ac_class);
        m_edit->setPlaceholderText(ac_class);
        m_edit->setText(q.value("ac_value").toString());
        m_layout->addWidget(m_edit, layoutRow, 1, 1, 1);
      }
      layoutRow++;
    }
  }
  m_layout->setRowStretch(layoutRow, 1);
}

void CompanySettings::saveSectionConfig() {
  Qt::FindChildOptions opt = Qt::FindDirectChildrenOnly;
  QLineEdit *cpn = findChild<QLineEdit *>("COMPANY_FULLNAME", opt);
  if (cpn != nullptr) {
    config->setValue("company_fullname", cpn->text());
  }

  QStringList queries;
  QList<QLineEdit *> le = findChildren<QLineEdit *>(QString(), opt);
  for (int i = 0; i < le.count(); i++) {
    QLineEdit *me = le.at(i);
    QString update("UPDATE antiquacrm_company SET");
    update.append(" ac_value='" + me->text().trimmed() + "'");
    update.append(" WHERE ac_class='" + me->objectName() + "';");
    queries.append(update);
  }
  QList<QTextEdit *> lt = findChildren<QTextEdit *>(QString(), opt);
  for (int i = 0; i < lt.count(); i++) {
    QTextEdit *me = lt.at(i);
    QString update("UPDATE antiquacrm_company SET");
    update.append(" ac_value='" + me->toPlainText().trimmed() + "'");
    update.append(" WHERE ac_class='" + me->objectName() + "';");
    queries.append(update);
  }
  AntiquaCRM::ASqlCore sql(this);
  sql.query(queries.join("\n"));
  if (!sql.lastError().isEmpty()) {
    qWarning("SQL-ERROR Company save ...");
    qDebug() << Q_FUNC_INFO << sql.lastError();
  }
}
