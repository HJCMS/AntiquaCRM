// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "companywidget.h"

CompanyWidget::CompanyWidget(QWidget *parent) : QWidget{parent} {
  layout = new QGridLayout(this);
  QString _txt =
      tr("On this page, configure your company presentation settings.");
  layout->addWidget(infoCell( _txt), p_row++, 0, 1, 2);
  setLayout(layout);
}

QLabel *CompanyWidget::infoCell(const QString &title) {
  QLabel *lb = new QLabel(title + ":", this);
  return lb;
}

void CompanyWidget::saveConfig() {
  Qt::FindChildOptions opt = Qt::FindDirectChildrenOnly;
  AntiquaCRM::AInputWidget *cpn =
      findChild<AntiquaCRM::AInputWidget *>("COMPANY_FULLNAME", opt);
  if (cpn != nullptr) {
    AntiquaCRM::ASettings cfg(this);
    cfg.setValue("company_fullname", cpn->getValue().toString());
  }

  QStringList queries;
  QList<AntiquaCRM::AInputWidget *> le =
      findChildren<AntiquaCRM::AInputWidget *>(QString(), opt);
  for (int i = 0; i < le.count(); i++) {
    AntiquaCRM::AInputWidget *me = le.at(i);
    QString update("UPDATE antiquacrm_company SET");
    update.append(" ac_value='" + me->getValue().toString() + "'");
    update.append(" WHERE ac_class='" + me->objectName() + "';");
    queries.append(update);
  }
  AntiquaCRM::ASqlCore sql(this);
  sql.query(queries.join("\n"));
  if (!sql.lastError().isEmpty()) {
    qWarning("SQL-ERROR Company save ...");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << sql.lastError();
#endif
    emit sendSaved(false);
  } else {
    emit sendSaved(true);
  }
}

bool CompanyWidget::loadConfig() {
  bool _status = false;
  const QString _sql("SELECT * FROM antiquacrm_company ORDER BY ac_sort;");
  AntiquaCRM::ASqlCore c_sql(this);
  QSqlQuery _q = c_sql.query(_sql);
  if (_q.size() > 0) {
    _status = true;
    while (_q.next()) {
      int _type = _q.value("ac_edit").toInt();
      if (_type == 1) {
        layout->addWidget(infoCell(_q.value("ac_info").toString()), // label
                          p_row, 0, 1, 1, (Qt::AlignTop | Qt::AlignRight));
        AntiquaCRM::TextField *m_edit = new AntiquaCRM::TextField(this);
        m_edit->setObjectName(_q.value("ac_class").toString());
        m_edit->setValue(_q.value("ac_value").toString());
        layout->addWidget(m_edit, p_row, 1, 1, 1);
      } else {
        layout->addWidget(infoCell(_q.value("ac_info").toString()), // label
                          p_row, 0, 1, 1, Qt::AlignRight);
        AntiquaCRM::TextLine *m_edit = new AntiquaCRM::TextLine(this);
        m_edit->setObjectName(_q.value("ac_class").toString());
        m_edit->setValue(_q.value("ac_value").toString());
        layout->addWidget(m_edit, p_row, 1, 1, 1);
      }
      p_row++;
    }
  }
  layout->setRowStretch(p_row, 1);
  return _status;
}
