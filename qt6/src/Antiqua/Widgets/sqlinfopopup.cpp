// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sqlinfopopup.h"
#include "alabel.h"

#include <AntiquaCRM>
#include <QMetaType>
#include <QtSql>

namespace AntiquaCRM {

SqlInfoPopUp::SqlInfoPopUp(QWidget *parent) : QDialog{parent, Qt::Tool} {
  setWindowTitle(tr("Connectioninfo"));
  setMinimumSize(380, 160);
  setSizeGripEnabled(true);
  layout = new QGridLayout(this);
  layout->setColumnStretch(3, 1);
  setLayout(layout);
}

void SqlInfoPopUp::addRow(const QString &title, QString &value) {
  ALabel *m_a = new ALabel(title, ALabel::MiddleRight, this);
  layout->addWidget(m_a, row_count, 0, 1, 1);
  ALabel *m_b = new ALabel(":", ALabel::MiddleCenter, this);
  layout->addWidget(m_b, row_count, 1, 1, 1);
  ALabel *m_c = new ALabel(value, ALabel::MiddleLeft, this);
  layout->addWidget(m_c, row_count++, 2, 1, 1);
}

int SqlInfoPopUp::exec() {
  AntiquaCRM::ASqlCore sql(this);
  QSqlQuery _query = sql.query(
      AntiquaCRM::ASqlFiles::queryStatement("query_connection_info"));
  if (_query.size() > 0) {
    QSqlRecord r = _query.record();
    while (_query.next()) {
      for (int i = 0; i < r.count(); i++) {
        QSqlField _field = r.field(i);
        QString _name = AntiquaCRM::AUtil::ucFirst(_field.name());
        QMetaType _type = _query.value(_field.name()).metaType();
        if (_type.id() == QMetaType::QString) {
          QString _value = _query.value(_field.name()).toString();
          addRow(_name, _value);
        }
      }
    }
    layout->setRowStretch(row_count++, 1);
  }
  return QDialog::exec();
}

} // namespace AntiquaCRM
