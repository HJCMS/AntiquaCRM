// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sqlinfopopup.h"
#include "alabel.h"

#include <AntiquaCRM>
#include <QMetaType>
#include <QtSql>

namespace AntiquaCRM
{

SqlInfoPopUp::SqlInfoPopUp(QWidget* parent) : QDialog{parent, Qt::Tool} {
  // Translated values
  p_trMap.insert("DB_TLS_SECURED", tr("Secure connection with SSL/TLS."));
  p_trMap.insert("DB_KRB_SECURED", tr("Secure connection with Kerberos."));
  p_trMap.insert("DB_UNSECURED", tr("Warning, unsecured connection!"));
  // Translated key names
  p_trMap.insert("usename", tr("Users"));
  p_trMap.insert("application_name", tr("Application"));
  p_trMap.insert("client_addr", tr("Address"));
  p_trMap.insert("status", tr("Status"));
  p_trMap.insert("connection", tr("Connection"));
  // Layout
  setWindowTitle(tr("Connectioninfo"));
  setMinimumSize(380, 160);
  setSizeGripEnabled(true);
  layout = new QGridLayout(this);
  layout->setColumnStretch(3, 1);
  setLayout(layout);
}

const QString SqlInfoPopUp::translate(const QString& str) const {
  const QString _v = p_trMap.value(str.trimmed());
  return (_v.length() < 1) ? str : _v;
}

void SqlInfoPopUp::addRow(const QString& title, const QString& value) {
  ALabel* m_a = new ALabel(title, ALabel::MiddleRight, this);
  layout->addWidget(m_a, row_count, 0, 1, 1);
  ALabel* m_b = new ALabel(":", ALabel::MiddleCenter, this);
  layout->addWidget(m_b, row_count, 1, 1, 1);
  ALabel* m_c = new ALabel(value, ALabel::MiddleLeft, this);
  layout->addWidget(m_c, row_count++, 2, 1, 1);
}

int SqlInfoPopUp::exec() {
  AntiquaCRM::ASqlCore sql(this);
  QSqlQuery _query = sql.query(AntiquaCRM::ASqlFiles::queryStatement("query_connection_info"));
  if (_query.size() > 0) {
    QSqlRecord r = _query.record();
    while (_query.next()) {
      for (int i = 0; i < r.count(); i++) {
        QSqlField _field = r.field(i);
        const QString _title = _field.name();
        const QMetaType _type = _query.value(_title).metaType();
        if (_type.id() == QMetaType::QString) {
          QString _value = _query.value(_title).toString();
          addRow(translate(_title), translate(_value));
        }
      }
    }
    layout->setRowStretch(row_count++, 1);
    _query.clear();
  }
  return QDialog::exec();
}

} // namespace AntiquaCRM
